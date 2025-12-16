/*
 * common_resource_data.h
 *
 *  Created on: 16 déc. 2025
 *      Author: ja.MOPAKA
 */

#ifndef COMMON_RESOURCE_H_
#define COMMON_RESOURCE_H_
#include "tx_port.h"
#include <math.h>


UINT usb_used_total_bytes_get(ULONG64 *p_used, ULONG64 *p_total);
void usb_usage_to_string(char *out, size_t out_sz, ULONG64 used_bytes, ULONG64 total_bytes);


#endif /* COMMON_RESOURCE_H_ */
