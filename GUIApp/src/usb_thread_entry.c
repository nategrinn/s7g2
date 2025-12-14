#include "usb_thread.h"
#include "tx_api.h"
#include "ux_api.h"
#include "ux_host_class_storage.h"
#include "fx_api.h"
#include "hmi_state.h"

/* Event flags created in configurator (USB Plug Event Flags) */
extern TX_EVENT_FLAGS_GROUP         g_usb_plug_events;

/* Bits for events */
#define USB_PLUG_EVENT_INSERT       (1U << 0)
#define USB_PLUG_EVENT_REMOVE       (1U << 1)

/* Globals for the mounted USB device */
static UX_HOST_CLASS_STORAGE_MEDIA  * gp_usb_storage_media = UX_NULL;
FX_MEDIA                            * g_usb_media          = UX_NULL;

extern volatile hmi_state_t         g_hmi_state;

UINT usb_host_plug_event_notification(ULONG event,
                                      UX_HOST_CLASS * host_class,
                                      VOID * instance)
{
    UINT status = UX_SUCCESS;
    UX_HOST_CLASS_STORAGE * storage = UX_NULL;

    SSP_PARAMETER_NOT_USED(host_class);

    if (instance != UX_NULL)
    {
        storage = (UX_HOST_CLASS_STORAGE *) instance;
    }

    switch (event)
    {
        case UX_DEVICE_INSERTION:
            if (storage != UX_NULL)
            {
                /* Get both the storage_media and the FX_MEDIA pointer */
                status = ux_system_host_storage_fx_media_get(storage,
                                                             &gp_usb_storage_media,
                                                             &g_usb_media);
                if (status == UX_SUCCESS)
                {
                    /* Notify usb_thread: device inserted & media available */
                    tx_event_flags_set(&g_usb_plug_events,
                                       USB_PLUG_EVENT_INSERT,
                                       TX_OR);
                }
                else
                {
                    gp_usb_storage_media = UX_NULL;
                    g_usb_media         = UX_NULL;
                }
            }

            break;

        case UX_DEVICE_REMOVAL:
            /* Invalidate our pointers */
            gp_usb_storage_media = UX_NULL;
            g_usb_media         = UX_NULL;

            /* Notify usb_thread: device removed */
            tx_event_flags_set(&g_usb_plug_events,
                               USB_PLUG_EVENT_REMOVE,
                               TX_OR);
            break;

        default:
            /* Ignore other events */
            break;
    }

    /* We handled the event */
    return UX_SUCCESS;
}

/*******************************************************************************************************************//**
 * USB Thread entry function
 **********************************************************************************************************************/
void usb_thread_entry(void)
{
    UINT        status;
    ULONG       actual_flags;
    FX_FILE     file;

    /* At startup, tell the HMI we are waiting for USB */
    g_hmi_state = HMI_WAIT_USB;

    while (1)
    {
        /* Wait for insert/remove events from the callback */
        status = tx_event_flags_get(&g_usb_plug_events,
                                    USB_PLUG_EVENT_INSERT | USB_PLUG_EVENT_REMOVE,
                                    TX_OR_CLEAR,
                                    &actual_flags,
                                    TX_WAIT_FOREVER);
        if (status != TX_SUCCESS)
        {
            continue;
        }

        if (actual_flags & USB_PLUG_EVENT_INSERT)
        {
            /* USB just plugged in: switch HMI to "mounting" */
            g_hmi_state = HMI_USB_MOUNTING;
        }

        if( g_usb_media != FX_NULL )
        {
            /* Optional sanity check: media looks valid */
            if( g_usb_media->fx_media_id == FX_MEDIA_ID )
            {
                /* -- TEST: create/write a file on the USB key -- */

                /* Ignore error if the file already exists */
                fx_file_create(g_usb_media, "usb_test.txt");

                status = fx_file_open(g_usb_media,
                                      &file,
                                      "usb_test.txt",
                                      FX_OPEN_FOR_WRITE);

                if( status == FX_SUCCESS )
                {
                    CHAR buffer[] = "Hello world";

                    fx_file_seek(&file, 0);         /* Go to start or end */
                    fx_file_write(&file, buffer, sizeof(buffer) - 1);
                    fx_file_close(&file);
                    fx_media_flush(g_usb_media);
                }

                /* If you got here, onsider USB ready */
                g_hmi_state = HMI_USB_READY;
            }
            else
            {
                /* Something went wrong */
                g_hmi_state = HMI_WAIT_USB;
            }
        }

        if (actual_flags & USB_PLUG_EVENT_REMOVE)
        {
            /* When USB is removed, go back to WAIT_USB */
            g_hmi_state = HMI_WAIT_USB;
        }
    }
}
