/* generated thread header file - do not edit */
#ifndef USB_THREAD_H_
#define USB_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void usb_thread_entry(void);
                #else
extern void usb_thread_entry(void);
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern TX_EVENT_FLAGS_GROUP g_usb_plug_events;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* USB_THREAD_H_ */
