#include "hal_data.h"
#include "auth_qspi_store.h"
#include <string.h>

#define AUTH_QSPI_MMAP_BASE   (0x60000000u)

#define AUTH_QSPI_TOTAL_SIZE   (8u * 1024u * 1024u) /* W25Q64: 8 MiB */

/* One-time open (QSPI driver is thread-safe enough for occasional config writes;
 * if you do more, protect with a mutex).
 */
static uint8_t s_qspi_opened = 0;

static ssp_err_t qspi_open_once(void)
{
    if (s_qspi_opened) return SSP_SUCCESS;
    ssp_err_t err = g_qspi0.p_api->open(g_qspi0.p_ctrl, g_qspi0.p_cfg);
    if (err == SSP_SUCCESS) s_qspi_opened = 1;
    return err;
}


static ssp_err_t qspi_validate_range(uint32_t offset, uint32_t len)
{
    if ((offset >= AUTH_QSPI_TOTAL_SIZE) || (len > AUTH_QSPI_TOTAL_SIZE) || ((offset + len) > AUTH_QSPI_TOTAL_SIZE))
    {
        return SSP_ERR_INVALID_ARGUMENT;
    }
    return SSP_SUCCESS;
}
ssp_err_t auth_qspi_read(uint32_t offset, void *dst, uint32_t len)
{
    ssp_err_t vr = qspi_validate_range(offset, len);
    if (vr != SSP_SUCCESS) return vr;
    ssp_err_t err = qspi_open_once();
    if (err != SSP_SUCCESS) return err;
    return g_qspi0.p_api->read(g_qspi0.p_ctrl, (uint8_t *)(AUTH_QSPI_MMAP_BASE + offset), (uint8_t*)dst, len);
}

ssp_err_t auth_qspi_erase_sector(uint32_t offset)
{
    ssp_err_t vr = qspi_validate_range(offset, AUTH_QSPI_SECTOR_SIZE);
    if (vr != SSP_SUCCESS) return vr;
    ssp_err_t err = qspi_open_once();
    if (err != SSP_SUCCESS) return err;

    /* Must be sector-aligned for a 4KB erase. */
    uint32_t addr = offset - (offset % AUTH_QSPI_SECTOR_SIZE);
    return g_qspi0.p_api->erase(g_qspi0.p_ctrl, addr, AUTH_QSPI_SECTOR_SIZE);
}

ssp_err_t auth_qspi_program(uint32_t offset, const void *src, uint32_t len)
{
    ssp_err_t vr = qspi_validate_range(offset, len);
    if (vr != SSP_SUCCESS) return vr;
    ssp_err_t err = qspi_open_once();
    if (err != SSP_SUCCESS) return err;

    const uint8_t *p = (const uint8_t*)src;
    uint32_t addr = offset;

    while (len > 0u)
    {
        uint32_t page_off = addr % AUTH_QSPI_PAGE_SIZE;
        uint32_t room = AUTH_QSPI_PAGE_SIZE - page_off;
        uint32_t chunk = (len < room) ? len : room;

        err = g_qspi0.p_api->pageProgram(g_qspi0.p_ctrl, (uint8_t *)(AUTH_QSPI_MMAP_BASE + addr), (uint8_t *)p, chunk);
        if (err != SSP_SUCCESS) return err;

        addr += chunk;
        p += chunk;
        len -= chunk;
    }
    return SSP_SUCCESS;
}
