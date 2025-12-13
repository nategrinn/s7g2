/*
 * auth_qspi_store.c
 *
 *  Created on: Dec 12, 2025
 *      Author: mopak
 */
#include "hal_data.h"
#include "auth_qspi.h"
#include <string.h>

#define AUTH_QSPI_OFFSET        (0x007FF000u)   // last 4KB of 8MB
#define AUTH_QSPI_SECTOR_SIZE   (0x1000u)       // 4KB
#define QSPI_PAGE_SIZE          (256u)


static uint8_t qspi_opened = 0;

static ssp_err_t qspi_open_once(void)
{
    if (qspi_opened)
        return SSP_SUCCESS;

    ssp_err_t err = g_qspi0.p_api->open(g_qspi0.p_ctrl, g_qspi0.p_cfg);
    if (err == SSP_SUCCESS)
        qspi_opened = 1;

    return err;
}

static ssp_err_t qspi_erase_auth_sector(void)
{
    ssp_err_t err = qspi_open_once();
    if (err != SSP_SUCCESS)
        return err;

    return g_qspi0.p_api->erase(
        g_qspi0.p_ctrl,
        AUTH_QSPI_OFFSET,
        AUTH_QSPI_SECTOR_SIZE
    );
}

static ssp_err_t qspi_write_paged(uint32_t addr, const uint8_t * data, uint32_t length)
{
    ssp_err_t err = qspi_open_once();
    if (err != SSP_SUCCESS)
        return err;

    while (length > 0)
    {
        uint32_t page_offset = addr % QSPI_PAGE_SIZE;
        uint32_t space       = QSPI_PAGE_SIZE - page_offset;
        uint32_t chunk       = (length < space) ? length : space;

        err = g_qspi0.p_api->pageProgram(
            g_qspi0.p_ctrl,
            addr,
            data,
            chunk
        );
        if (err != SSP_SUCCESS)
            return err;

        addr   += chunk;
        data   += chunk;
        length -= chunk;
    }

    return SSP_SUCCESS;
}

static ssp_err_t qspi_read(uint32_t addr, void * dst, uint32_t len)
{
    ssp_err_t err = qspi_open_once();
    if (err != SSP_SUCCESS)
        return err;

    return g_qspi0.p_api->read(
        g_qspi0.p_ctrl,
        (uint8_t *)dst,
        addr,
        len
    );
}

ssp_err_t auth_qspi_save(const auth_record_t * rec)
{
    ssp_err_t err;

    err = qspi_erase_auth_sector();
    if (err != SSP_SUCCESS)
        return err;

    return qspi_write_paged(
        AUTH_QSPI_OFFSET,
        (const uint8_t *)rec,
        sizeof(auth_record_t)
    );
}

ssp_err_t auth_qspi_load(auth_record_t * out)
{
    ssp_err_t err = qspi_read(AUTH_QSPI_OFFSET, out, sizeof(*out));
    if (err != SSP_SUCCESS)
        return err;

    if (out->magic != AUTH_MAGIC || out->version != AUTH_VERSION)
        return SSP_ERR_INVALID_ARGUMENT;

    uint32_t saved_crc = out->crc32;
    out->crc32 = 0;

    uint32_t calc_crc = crc32_compute(out, sizeof(*out));
    out->crc32 = saved_crc;

    if (calc_crc != saved_crc)
        return SSP_ERR_INVALID_ARGUMENT;

    return SSP_SUCCESS;
}

