#pragma once
#include <stdint.h>
#include "ssp_common_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------- QSPI auth storage layout ----------
 * Default: last 4KB sector of an 8MB flash (W25Q64 -> 8MB).
 * If you use QSPI for assets/XIP, override AUTH_QSPI_OFFSET in your build.
 */
#ifndef AUTH_QSPI_TOTAL_SIZE
#define AUTH_QSPI_TOTAL_SIZE   (8u * 1024u * 1024u)
#endif

#ifndef AUTH_QSPI_SECTOR_SIZE
#define AUTH_QSPI_SECTOR_SIZE  (4u * 1024u)
#endif

#ifndef AUTH_QSPI_OFFSET
#define AUTH_QSPI_OFFSET       (AUTH_QSPI_TOTAL_SIZE - AUTH_QSPI_SECTOR_SIZE) /* 0x007FF000 */
#endif

#define AUTH_QSPI_PAGE_SIZE    (256u)

ssp_err_t auth_qspi_read(uint32_t offset, void *dst, uint32_t len);
ssp_err_t auth_qspi_erase_sector(uint32_t offset);
ssp_err_t auth_qspi_program(uint32_t offset, const void *src, uint32_t len);

#ifdef __cplusplus
}
#endif
