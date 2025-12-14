/* generated thread header file - do not edit */
#ifndef NETWORK_THREAD_H_
#define NETWORK_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void network_thread_entry(void);
                #else
extern void network_thread_entry(void);
#endif
#include "sf_crypto_trng.h"
#include "r_hash_api.h"
#include "sf_crypto_hash.h"
#include "nxd_http_server.h"
#include "nxd_dhcp_client.h"
#ifdef __cplusplus
extern "C" {
#endif
extern sf_crypto_trng_instance_t g_sf_crypto_trng0;
void g_sf_crypto_trng0_err_callback(void *p_instance, void *p_data);
void sf_crypto_trng_init0(void);
extern const hash_instance_t g_sce_hash_0;
extern sf_crypto_hash_instance_t g_sf_crypto_hash0;

/* Crypto HASH Framework Initialization Function for the instance 'g_sf_crypto_hash0'. */
void sf_crypto_hash_init0(void);
/* Crypto HASH Framework Error Callback Function for the instance 'g_sf_crypto_hash0'. */
void g_sf_crypto_hash0_err_callback(void *p_instance, void *p_data);
extern NX_HTTP_SERVER g_http_server0;
#if !defined(NULL)
UINT NULL(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, CHAR **name, CHAR **password, CHAR **realm);
#endif
#if !defined(http_request_notify)
UINT http_request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr);
#endif
void g_http_server0_err_callback(void *p_instance, void *p_data);
void http_server_init0(void);
extern NX_DHCP g_dhcp_client0;
void g_dhcp_client0_err_callback(void *p_instance, void *p_data);
void dhcp_client_init0(void);

#define DHCP_USR_OPT_ADD_ENABLE_g_dhcp_client0 (0)
#define DHCP_USR_OPT_ADD_FUNCTION_ENABLE_g_dhcp_client0 (1)

#if DHCP_USR_OPT_ADD_ENABLE_g_dhcp_client0
            UINT dhcp_user_option_add_client0(NX_DHCP *dhcp_ptr, UINT iface_index, UINT message_type, UCHAR *user_option_ptr, UINT *user_option_length);
            #if DHCP_USR_OPT_ADD_FUNCTION_ENABLE_g_dhcp_client0
            extern UCHAR g_dhcp_client0_opt_num;
            extern CHAR *g_dhcp_client0_opt_val;
            #endif
            #endif
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* NETWORK_THREAD_H_ */
