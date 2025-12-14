/*
 * storage.h
 *
 *  Created on: 10 déc. 2025
 *      Author: ja.MOPAKA
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include "fx_api.h"

/* Pointer to mounted USB FileX media (set by usb_thread) */
extern FX_MEDIA * g_usb_media;

/* Simple helper so other code can check if USB is ready */
static inline UINT storage_is_ready(void)
{
    return (g_usb_media != FX_NULL);
}

#endif /* STORAGE_H_ */
