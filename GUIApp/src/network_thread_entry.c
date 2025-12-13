#include "network_thread.h"
#include "tx_api.h"
#include "nx_api.h"
#include "nxd_dhcp_client.h"
#include "nxd_http_server.h"   // or nx_web_http_server.h if your type is NX_WEB_HTTP_SERVER
#include "hmi_state.h"
#include "network_info.h"


extern NX_IP            g_ip0;
extern NX_DHCP          g_dhcp_client0;
extern NX_HTTP_SERVER   g_http_server0;   // adjust to NX_WEB_HTTP_SERVER if needed

volatile ULONG          g_ipv4_address  = 0;
volatile ULONG          g_ipv4_mask     = 0;
volatile UINT           g_ip_info_valid = 0;

static void wait_for_ip_address(void)
{
    UINT  status;
    ULONG actual_status;
    UINT  retries = 0;

    /* 0) Wait for link up (cable + PHY) */
    do
    {
        status = nx_ip_status_check(&g_ip0,
                                    NX_IP_LINK_ENABLED,
                                    &actual_status,
                                    NX_IP_PERIODIC_RATE);
        retries++;
        if (retries > 10)
        {
            /* Link never came up: give up here or set a static IP anyway */
            return;
        }
    } while (status != NX_SUCCESS);

    /* 1) Start DHCP client */
    status = nx_dhcp_start(&g_dhcp_client0);
    if ((status != NX_SUCCESS) && (status != NX_DHCP_ALREADY_STARTED))
    {
        /* DHCP failed to start → fallback to static IP */
        nx_ip_address_set(&g_ip0,
                          IP_ADDRESS(192, 168, 0, 50),
                          IP_ADDRESS(255, 255, 255, 0));
        return;
    }

    /* 2) Wait for IP address to be resolved (limit retries to avoid infinite loop) */
    retries = 0;
    do
    {
        status = nx_ip_status_check(&g_ip0,
                                    NX_IP_ADDRESS_RESOLVED,
                                    &actual_status,
                                    NX_IP_PERIODIC_RATE * 5);  /* wait ~5s */

        retries++;
        if (retries > 10)
        {
            /* DHCP didn’t give an IP → stop DHCP and set static IP */
            nx_dhcp_stop(&g_dhcp_client0);

            nx_ip_address_set(&g_ip0,
                              IP_ADDRESS(192, 168, 0, 50),
                              IP_ADDRESS(255, 255, 255, 0));
            return;
        }
    } while ((status != NX_SUCCESS) ||
             ((actual_status & NX_IP_ADDRESS_RESOLVED) == 0));

    /* 3) Read back IP for debug */
    status = nx_ip_address_get(&g_ip0, &g_ipv4_address, &g_ipv4_mask);
    if (status == NX_SUCCESS && g_ipv4_address != 0)
    {
        g_ip_info_valid = 1;
    }
    else
    {
        g_ip_info_valid = 0;
    }
}

#if 0 // Force static IP
void network_thread_entry(void)
{
    UINT  status;
    ULONG ip;
    ULONG mask;

    /* 1) Force a static IP just for debugging.
       Pick something in your LAN range, e.g. 192.168.1.50 / 255.255.255.0  */

    nx_ip_address_set(&g_ip0,
                      IP_ADDRESS(192, 168, 1, 50),
                      IP_ADDRESS(255, 255, 255, 0));

    nx_ip_address_get(&g_ip0, &g_ipv4_address, &g_ipv4_mask);
    g_ip_info_valid = (g_ipv4_address != 0);

    /* 2) Wait for USB ready as before */
    while (g_hmi_state != HMI_USB_READY)
    {
        tx_thread_sleep(10);
    }

    /* 3) Start HTTP server */
    status = nx_http_server_start(&g_http_server0);
    if (status == NX_SUCCESS)
    {
        g_hmi_state = HMI_HTTP_RUNNING;
    }

    while (1)
    {
        tx_thread_sleep(100);
    }
}
#else
/* Network Thread entry function */
void network_thread_entry(void)
{
    UINT    status;

    /* 1) Wait until IP address is acquired via DHCP */
    wait_for_ip_address();

    /* 2) Wait for USB to be ready (set by usb_thread) */
    while (g_hmi_state != HMI_USB_READY)
    {
        tx_thread_sleep(10);
    }

    /* 3) Start the HTTP server */
    status = nx_http_server_start(&g_http_server0);
    if (status == NX_SUCCESS)
    {
        g_hmi_state = HMI_HTTP_RUNNING;
    }
    else
    {
        /* If it fails, you could keep state at USB_READY or add an error state */
    }

    /* 4) Nothing else to do here; HTTP Server runs in its own thread */
    while (1)
    {
        tx_thread_sleep(100);
    }
}
#endif
