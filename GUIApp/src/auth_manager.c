#include "auth_manager.h"

#include "hal_data.h"
#include "common_data.h"

#include "sha256_mini.h"

#include <string.h>
#include <stdio.h>

/*
 * QSPI HAL module guide shows API call order like:
 *   g_qspi0.p_api->read(g_qspi0.p_ctrl, (uint8_t*)QSPI_DEVICE_ADDRESS, readBuffer, len);
 *   g_qspi0.p_api->pageProgram(g_qspi0.p_ctrl, (uint8_t*)QSPI_DEVICE_ADDRESS, writeBuffer, len);
 *   g_qspi0.p_api->sectorErase(g_qspi0.p_ctrl, (uint8_t*)QSPI_DEVICE_ADDRESS);
 *
 * And notes that when the driver is CLOSED, QSPI is mapped at 0x60000000 and can be read like memory.
 */

/* ---- Configuration (override with -D in build if needed) ---- */
#ifndef AUTH_QSPI_MMAP_BASE
#define AUTH_QSPI_MMAP_BASE   (0x60000000u)
#endif

/* SK-S7G2 user manual indicates 64Mb (8MB) QSPI flash.
 * Put auth record in the last 4KiB sector by default.
 */
#ifndef AUTH_QSPI_OFFSET
#define AUTH_QSPI_OFFSET      (0x007FF000u)   /* 8MB - 4KB */
#endif

#ifndef AUTH_QSPI_SECTOR_SIZE
#define AUTH_QSPI_SECTOR_SIZE (4096u)
#endif

#define AUTH_QSPI_ADDR_PTR()  ((uint8_t *)(AUTH_QSPI_MMAP_BASE + AUTH_QSPI_OFFSET))

#define AUTH_MAGIC            (0x41555448u) /* 'AUTH' */
#define AUTH_VERSION          (1u)
#define AUTH_USERNAME         "admin"
#define AUTH_HASH_ITERS       (5000u)

/* ---- Diagnostics (optional) ---- */
static const char * s_last_qspi_op = "";
static uint32_t     s_last_qspi_err = 0u;

const char * auth_last_qspi_op(void)      { return s_last_qspi_op; }
uint32_t     auth_last_qspi_err_u32(void) { return s_last_qspi_err; }

static void qspi_set_diag(const char *op, uint32_t err)
{
    s_last_qspi_op  = op;
    s_last_qspi_err = err;
}

/* ---- Record format ---- */
typedef struct
{
    uint32_t magic;
    uint32_t version;
    uint32_t iters;
    uint8_t  salt[16];
    uint8_t  hash[32];
    uint32_t crc32;
} auth_record_t;

/* Simple CRC32 (polynomial 0xEDB88320) */
static uint32_t crc32_calc(const void *data, uint32_t len)
{
    const uint8_t *p = (const uint8_t *) data;
    uint32_t crc = 0xFFFFFFFFu;
    for (uint32_t i = 0; i < len; i++)
    {
        crc ^= p[i];
        for (uint32_t b = 0; b < 8; b++)
        {
            uint32_t mask = (uint32_t)(-(int)(crc & 1u));
            crc = (crc >> 1) ^ (0xEDB88320u & mask);
        }
    }
    return ~crc;
}

/* ---- QSPI helpers ---- */
static int qspi_open(void)
{
    ssp_err_t err = g_qspi0.p_api->open(g_qspi0.p_ctrl, g_qspi0.p_cfg);
    if (SSP_SUCCESS != err)
    {
        qspi_set_diag("open", err);
        return -1;
    }
    return 0;
}

static void qspi_close(void)
{
    (void) g_qspi0.p_api->close(g_qspi0.p_ctrl);
}

static int qspi_wait_ready(void)
{
    /* statusGet returns busy flag in a bool in the official examples */
    bool busy = true;
    ssp_err_t err = SSP_SUCCESS;

    /* Worst-case guard: do not loop forever. */
    for (uint32_t i = 0; i < 200000u; i++)
    {
        err = g_qspi0.p_api->statusGet(g_qspi0.p_ctrl, &busy);
        if (SSP_SUCCESS != err)
        {
            qspi_set_diag("statusGet", err);
            return -1;
        }
        if (!busy)
        {
            return 0;
        }
    }

    qspi_set_diag("statusTimeout", 0xFFFFFFFFu);
    return -1;
}

static int qspi_erase_4k(uint8_t *addr)
{
    ssp_err_t err = g_qspi0.p_api->erase(g_qspi0.p_ctrl, addr, AUTH_QSPI_SECTOR_SIZE);
    if (SSP_SUCCESS != err) { qspi_set_diag("erase", err); return -1; }
    return qspi_wait_ready();
}

static int qspi_program(uint8_t *addr, const uint8_t *src, uint32_t len)
{
    /* Program by pages (256B typical). */
    uint32_t remaining = len;
    uint32_t off = 0;

    while (remaining)
    {
        uint32_t chunk = remaining;
        if (chunk > 256u) chunk = 256u;

        ssp_err_t err = g_qspi0.p_api->pageProgram(g_qspi0.p_ctrl,
                                                   (uint8_t *)(addr + off),
                                                   (uint8_t *)(src + off),
                                                   chunk);
        if (SSP_SUCCESS != err) { qspi_set_diag("pageProgram", err); return -1; }
        if (qspi_wait_ready() != 0) return -1;

        off += chunk;
        remaining -= chunk;
    }
    return 0;
}

/* ---- Hashing ---- */
static void random_salt(uint8_t salt[16])
{
    /* If you have a TRNG, use it. Otherwise use a very simple LFSR-ish fallback. */
    static uint32_t x = 0x12345678u;
    for (uint32_t i = 0; i < 16; i++)
    {
        x ^= x << 13; x ^= x >> 17; x ^= x << 5;
        salt[i] = (uint8_t) (x & 0xFFu);
    }
}

static void derive_hash(const uint8_t salt[16], const char *password, uint32_t iters, uint8_t out_hash[32])
{
    /* SHA256(salt || password) iterated */
    uint8_t buf[16 + 32]; /* salt + hash */
    uint8_t h[32];

    sha256_ctx_t ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, salt, 16);
    sha256_update(&ctx, (const uint8_t *) password, (uint32_t) strlen(password));
    sha256_final(&ctx, h);

    for (uint32_t i = 1; i < iters; i++)
    {
        memcpy(buf, salt, 16);
        memcpy(buf + 16, h, 32);

        sha256_init(&ctx);
        sha256_update(&ctx, buf, sizeof(buf));
        sha256_final(&ctx, h);
    }

    memcpy(out_hash, h, 32);
}

/* ---- Record read/write ---- */
static int auth_record_read(auth_record_t *out)
{
    if (!out) return -1;

    /* Read via memory-mapped region while QSPI driver is CLOSED (BSP puts QSPI in ROM access mode). */
    memcpy(out, AUTH_QSPI_ADDR_PTR(), sizeof(auth_record_t));

    if (out->magic != AUTH_MAGIC || out->version != AUTH_VERSION)
    {
        return -1;
    }

    uint32_t want = out->crc32;
    out->crc32 = 0;
    uint32_t have = crc32_calc(out, sizeof(auth_record_t));
    out->crc32 = want;

    return (have == want) ? 0 : -1;
}

static int auth_record_write(const auth_record_t *rec)
{
    if (!rec) return -1;

    if (qspi_open() != 0) return -1;

    int rc = 0;
    uint8_t *addr = AUTH_QSPI_ADDR_PTR();

    /* Erase sector then program. */
    if (qspi_erase_4k(addr) != 0) rc = -1;
    if (rc == 0 && qspi_program(addr, (const uint8_t *) rec, sizeof(auth_record_t)) != 0) rc = -1;

    qspi_close();
    return rc;
}

/* ---- Public API ---- */
int auth_init(void)
{
    /* Nothing heavy here; we read from the mapped region without opening QSPI. */
    return 0;
}

int auth_is_provisioned(void)
{
    auth_record_t rec;
    return (auth_record_read(&rec) == 0) ? 1 : 0;
}

int auth_verify_credentials(const char *username, const char *password)
{
    if (!username || !password) return 0;
    if (strcmp(username, AUTH_USERNAME) != 0) return 0;

    auth_record_t rec;
    if (auth_record_read(&rec) != 0) return 0;

    uint8_t calc[32];
    derive_hash(rec.salt, password, rec.iters, calc);

    return (memcmp(calc, rec.hash, 32u) == 0) ? 1 : 0;
}

/* Public wrapper you referenced in UI/HTTP code. */
int auth_verify_credentials_public(const char *username, const char *password)
{
    return auth_verify_credentials(username, password);
}

int auth_set_admin_password(const char *new_password)
{
    if (!new_password || new_password[0] == '\0') return -1;

    auth_record_t rec;
    memset(&rec, 0, sizeof(rec));

    rec.magic   = AUTH_MAGIC;
    rec.version = AUTH_VERSION;
    rec.iters   = AUTH_HASH_ITERS;

    random_salt(rec.salt);
    derive_hash(rec.salt, new_password, rec.iters, rec.hash);

    rec.crc32 = 0;
    rec.crc32 = crc32_calc(&rec, sizeof(rec));

    return auth_record_write(&rec);
}

int auth_reset_password(void)
{
    /* Erase only: write an invalid record */
    if (qspi_open() != 0) return -1;
    int rc = qspi_erase_4k(AUTH_QSPI_ADDR_PTR());
    qspi_close();
    return rc;
}

/* Optional: allow recovery checks (kept for compatibility) */
int auth_recovery_allowed(void) { return 1; }
int auth_is_configured(void)    { return auth_is_provisioned(); }
