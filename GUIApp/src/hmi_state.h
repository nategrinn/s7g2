/*
 * hmi_state.h
 *
 *  Created on: Dec 9, 2025
 *      Author: mopak
 */

#ifndef HMI_STATE_H_
#define HMI_STATE_H_

typedef enum
{
    HMI_WAIT_USB = 0,
    HMI_USB_MOUNTING,
    HMI_USB_READY,
    HMI_HTTP_RUNNING
} hmi_state_t;

/* Global HMI state (defined in main_thread_entry.c) */
extern volatile hmi_state_t g_hmi_state;

/* Implemented in main_thread_entry.c */
void hmi_screen_update(void);

#endif /* HMI_STATE_H_ */
