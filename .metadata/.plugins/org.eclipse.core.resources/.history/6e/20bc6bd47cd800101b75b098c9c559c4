/*
 * auth_qspi.h
 *
 *  Created on: Dec 12, 2025
 *      Author: mopak
 */
#pragma once
#include <stdint.h>

#ifndef AUTH_QSPI_AUTH_QSPI_H_
#define AUTH_QSPI_AUTH_QSPI_H_

#define AUTH_MAGIC   (0x4F534952u)   /* "OSIR" */
#define AUTH_VERSION (1u)

typedef struct __attribute__((packed))
{
    uint32_t magic;        // sanity check
    uint16_t version;
    uint16_t reserved;
    uint32_t iterations;   // e.g. 20000
    uint8_t  salt[16];     // random
    uint8_t  hash[32];     // derived key
    uint32_t crc32;        // integrity check
} auth_record_t;


#endif /* AUTH_QSPI_AUTH_QSPI_H_ */
