/* generated common source file - do not edit */
#include "common_data.h"

#ifndef NX_DISABLE_IPV6
#ifndef FILL_NXD_IPV6_ADDRESS
#define FILL_NXD_IPV6_ADDRESS(ipv6,f0,f1,f2,f3,f4,f5,f6,f7) do { \
                                                                       ipv6.nxd_ip_address.v6[0] = (((uint32_t)f0 << 16) & 0xFFFF0000) | ((uint32_t)f1 & 0x0000FFFF);\
                                                                       ipv6.nxd_ip_address.v6[1] = (((uint32_t)f2 << 16) & 0xFFFF0000) | ((uint32_t)f3 & 0x0000FFFF);\
                                                                       ipv6.nxd_ip_address.v6[2] = (((uint32_t)f4 << 16) & 0xFFFF0000) | ((uint32_t)f5 & 0x0000FFFF);\
                                                                       ipv6.nxd_ip_address.v6[3] = (((uint32_t)f6 << 16) & 0xFFFF0000) | ((uint32_t)f7 & 0x0000FFFF);\
                                                                       ipv6.nxd_ip_version       = NX_IP_VERSION_V6;\
                                                                   } while(0);
#endif /* FILL_NXD_IPV6_ADDRESS */
#endif
#define UX_HOST_INITIALIZE
#define USB_HOST_STORAGE_CLASS_REGISTER
#if defined(BSP_FEATURE_HAS_SCE_ON_S1)  /* Crypto on S1 */
            sce_interface_get_api_interfaces_t g_sce_selected_api_interfaces =
            {
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_128_ECB
                .aes128ecb_on_sce  = (uint32_t)&g_aes128ecb_on_sce,
            #else
                .aes128ecb_on_sce  = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CBC
                .aes128cbc_on_sce  = (uint32_t)&g_aes128cbc_on_sce,
            #else
                .aes128cbc_on_sce  = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CTR
                .aes128ctr_on_sce  = (uint32_t)&g_aes128ctr_on_sce,
            #else
                .aes128ctr_on_sce  = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_256_ECB
                .aes256ecb_on_sce  = (uint32_t)&g_aes256ecb_on_sce,
            #else
                .aes256ecb_on_sce  = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CBC
                .aes256cbc_on_sce  = (uint32_t)&g_aes256cbc_on_sce,
            #else
                .aes256cbc_on_sce  = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CTR
                .aes256ctr_on_sce  = (uint32_t)&g_aes256ctr_on_sce,
            #else
                .aes256ctr_on_sce  = 0,
            #endif
            #if R_SCE_SERVICES_TRNG
                .trng_on_sce       = (uint32_t)&g_trng_on_sce
            #else
                .trng_on_sce       = 0
            #endif
            };
            
            #elif defined(BSP_FEATURE_HAS_SCE5) /* SCE5 */

            sce_interface_get_api_interfaces_t g_sce_selected_api_interfaces =
            {
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_128_ECB
                .aes128ecb_on_sce     = (uint32_t)&g_aes128ecb_on_sce,
            #else
                .aes128ecb_on_sce     = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CBC
                .aes128cbc_on_sce     = (uint32_t)&g_aes128cbc_on_sce,
            #else
                .aes128cbc_on_sce     = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CTR
                .aes128ctr_on_sce     = (uint32_t)&g_aes128ctr_on_sce,
            #else
                .aes128ctr_on_sce     = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_128_GCM
                .aes128gcm_on_sce     = (uint32_t)&g_aes128gcm_on_sce,
            #else
                .aes128gcm_on_sce     = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_128_XTS
                .aes128xts_on_sce     = (uint32_t)&g_aes128xts_on_sce,
            #else
                .aes128xts_on_sce     = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_256_ECB
                .aes256ecb_on_sce     = (uint32_t)&g_aes256ecb_on_sce,
            #else
                .aes256ecb_on_sce     = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CBC
                .aes256cbc_on_sce     = (uint32_t)&g_aes256cbc_on_sce,
            #else
                .aes256cbc_on_sce     = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CTR
                .aes256ctr_on_sce     = (uint32_t)&g_aes256ctr_on_sce,
            #else
                .aes256ctr_on_sce     = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_256_GCM
                .aes256gcm_on_sce     = (uint32_t)&g_aes256gcm_on_sce,
            #else
                .aes256gcm_on_sce     = 0,
            #endif
            #if R_SCE_SERVICES_AES_PLAIN_TEXT_256_XTS
                .aes256xts_on_sce     = (uint32_t)&g_aes256xts_on_sce,
            #else
                .aes256xts_on_sce     = 0,
            #endif
            #if R_SCE_SERVICES_AES_WRAPPED_128_ECB
                .aes128ecb_on_sceHrk  = (uint32_t)&g_aes128ecb_on_sceHrk,
            #else
                .aes128ecb_on_sceHrk  = 0,
            #endif
            #if R_SCE_SERVICES_AES_WRAPPED_128_CBC
                .aes128cbc_on_sceHrk  = (uint32_t)&g_aes128cbc_on_sceHrk,
            #else
                .aes128cbc_on_sceHrk  = 0,
            #endif
            #if R_SCE_SERVICES_AES_WRAPPED_128_CTR
                .aes128ctr_on_sceHrk  = (uint32_t)&g_aes128ctr_on_sceHrk,
            #else
                .aes128ctr_on_sceHrk  = 0,
            #endif
            #if R_SCE_SERVICES_AES_WRAPPED_128_GCM
                .aes128gcm_on_sceHrk  = (uint32_t)&g_aes128gcm_on_sceHrk,
            #else
                .aes128gcm_on_sceHrk  = 0,
            #endif
            #if R_SCE_SERVICES_AES_WRAPPED_128_XTS
                .aes128xts_on_sceHrk  = (uint32_t)&g_aes128xts_on_sceHrk,
            #else
                .aes128xts_on_sceHrk  = 0,
            #endif
            #if R_SCE_SERVICES_AES_WRAPPED_256_ECB
                .aes256ecb_on_sceHrk  = (uint32_t)&g_aes256ecb_on_sceHrk,
            #else
                .aes256ecb_on_sceHrk  = 0,
            #endif
            #if R_SCE_SERVICES_AES_WRAPPED_256_CBC
                .aes256cbc_on_sceHrk  = (uint32_t)&g_aes256cbc_on_sceHrk,
            #else
                .aes256cbc_on_sceHrk  = 0,
            #endif
            #if R_SCE_SERVICES_AES_WRAPPED_256_CTR
                .aes256ctr_on_sceHrk  = (uint32_t)&g_aes256ctr_on_sceHrk,
            #else
                .aes256ctr_on_sceHrk  = 0,
            #endif
            #if R_SCE_SERVICES_AES_WRAPPED_256_GCM
                .aes256gcm_on_sceHrk  = (uint32_t)&g_aes256gcm_on_sceHrk,
            #else
                .aes256gcm_on_sceHrk  = 0,
            #endif
            #if R_SCE_SERVICES_AES_WRAPPED_256_XTS
                .aes256xts_on_sceHrk  = (uint32_t)&g_aes256xts_on_sceHrk,
            #else
                .aes256xts_on_sceHrk  = 0,
            #endif
            #if R_SCE_SERVICES_TRNG
                .trng_on_sce          = (uint32_t)&g_trng_on_sce
            #else
                .trng_on_sce          = 0
            #endif
            };
            
            #else /* SCE7 */

sce_interface_get_api_interfaces_t g_sce_selected_api_interfaces =
{
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_ECB
                .aes128ecb_on_sce     = (uint32_t)&g_aes128ecb_on_sce,
            #else
  .aes128ecb_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CBC
                .aes128cbc_on_sce     = (uint32_t)&g_aes128cbc_on_sce,
            #else
  .aes128cbc_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CTR
                .aes128ctr_on_sce     = (uint32_t)&g_aes128ctr_on_sce,
            #else
  .aes128ctr_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_GCM
                .aes128gcm_on_sce     = (uint32_t)&g_aes128gcm_on_sce,
            #else
  .aes128gcm_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_XTS
                .aes128xts_on_sce     = (uint32_t)&g_aes128xts_on_sce,
            #else
  .aes128xts_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_192_ECB
                .aes192ecb_on_sce     = (uint32_t)&g_aes192ecb_on_sce,
            #else
  .aes192ecb_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_192_CBC
                .aes192cbc_on_sce     = (uint32_t)&g_aes192cbc_on_sce,
            #else
  .aes192cbc_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_192_CTR
                .aes192ctr_on_sce     = (uint32_t)&g_aes192ctr_on_sce,
            #else
  .aes192ctr_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_192_GCM
                .aes192gcm_on_sce     = (uint32_t)&g_aes192gcm_on_sce,
            #else
  .aes192gcm_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_ECB
                .aes256ecb_on_sce     = (uint32_t)&g_aes256ecb_on_sce,
            #else
  .aes256ecb_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CBC
                .aes256cbc_on_sce     = (uint32_t)&g_aes256cbc_on_sce,
            #else
  .aes256cbc_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CTR
                .aes256ctr_on_sce     = (uint32_t)&g_aes256ctr_on_sce,
            #else
  .aes256ctr_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_GCM
                .aes256gcm_on_sce     = (uint32_t)&g_aes256gcm_on_sce,
            #else
  .aes256gcm_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_XTS
                .aes256xts_on_sce     = (uint32_t)&g_aes256xts_on_sce,
            #else
  .aes256xts_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_ECB
                .aes128ecb_on_sceHrk  = (uint32_t)&g_aes128ecb_on_sceHrk,
            #else
  .aes128ecb_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_CBC
                .aes128cbc_on_sceHrk  = (uint32_t)&g_aes128cbc_on_sceHrk,
            #else
  .aes128cbc_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_CTR
                .aes128ctr_on_sceHrk  = (uint32_t)&g_aes128ctr_on_sceHrk,
            #else
  .aes128ctr_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_GCM
                .aes128gcm_on_sceHrk  = (uint32_t)&g_aes128gcm_on_sceHrk,
            #else
  .aes128gcm_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_XTS
                .aes128xts_on_sceHrk  = (uint32_t)&g_aes128xts_on_sceHrk,
            #else
  .aes128xts_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_192_ECB
                .aes192ecb_on_sceHrk  = (uint32_t)&g_aes192ecb_on_sceHrk,
            #else
  .aes192ecb_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_192_CBC
                .aes192cbc_on_sceHrk  = (uint32_t)&g_aes192cbc_on_sceHrk,
            #else
  .aes192cbc_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_192_CTR
                .aes192ctr_on_sceHrk  = (uint32_t)&g_aes192ctr_on_sceHrk,
            #else
  .aes192ctr_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_192_GCM
                .aes192gcm_on_sceHrk  = (uint32_t)&g_aes192gcm_on_sceHrk,
            #else
  .aes192gcm_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_ECB
                .aes256ecb_on_sceHrk  = (uint32_t)&g_aes256ecb_on_sceHrk,
            #else
  .aes256ecb_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_CBC
                .aes256cbc_on_sceHrk  = (uint32_t)&g_aes256cbc_on_sceHrk,
            #else
  .aes256cbc_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_CTR
                .aes256ctr_on_sceHrk  = (uint32_t)&g_aes256ctr_on_sceHrk,
            #else
  .aes256ctr_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_GCM
                .aes256gcm_on_sceHrk  = (uint32_t)&g_aes256gcm_on_sceHrk,
            #else
  .aes256gcm_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_XTS
                .aes256xts_on_sceHrk  = (uint32_t)&g_aes256xts_on_sceHrk,
            #else
  .aes256xts_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_RSA_PLAIN_TEXT_1024
                .rsa1024_on_sce       = (uint32_t)&g_rsa1024_on_sce,
            #else
  .rsa1024_on_sce = 0,
#endif
#if R_SCE_SERVICES_RSA_PLAIN_TEXT_2048
                .rsa2048_on_sce       = (uint32_t)&g_rsa2048_on_sce,
            #else
  .rsa2048_on_sce = 0,
#endif
#if R_SCE_SERVICES_RSA_WRAPPED_1024
                .rsa1024_on_sce_hrk   = (uint32_t)&g_rsa1024_on_sce_hrk,
            #else
  .rsa1024_on_sce_hrk = 0,
#endif
#if R_SCE_SERVICES_RSA_WRAPPED_2048
                .rsa2048_on_sce_hrk   = (uint32_t)&g_rsa2048_on_sce_hrk,
            #else
  .rsa2048_on_sce_hrk = 0,
#endif
#if R_SCE_SERVICES_HASH_MD5
                .md5_hash_on_sce      = (uint32_t)&g_md5_hash_on_sce,
            #else
  .md5_hash_on_sce = 0,
#endif
#if R_SCE_SERVICES_HASH_SHA1
                .sha1_hash_on_sce     = (uint32_t)&g_sha1_hash_on_sce,
            #else
  .sha1_hash_on_sce = 0,
#endif
#if R_SCE_SERVICES_HASH_SHA256
                .sha256_hash_on_sce   = (uint32_t)&g_sha256_hash_on_sce,
            #else
  .sha256_hash_on_sce = 0,
#endif
#if R_SCE_SERVICES_ECC_PLAIN_TEXT_192
                .ecc192_on_sce        = (uint32_t)&g_ecc192_on_sce,
            #else
  .ecc192_on_sce = 0,
#endif
#if R_SCE_SERVICES_ECC_PLAIN_TEXT_224
                .ecc224_on_sce        = (uint32_t)&g_ecc224_on_sce,
            #else
  .ecc224_on_sce = 0,
#endif
#if R_SCE_SERVICES_ECC_PLAIN_TEXT_256
                .ecc256_on_sce        = (uint32_t)&g_ecc256_on_sce,
            #else
  .ecc256_on_sce = 0,
#endif
#if R_SCE_SERVICES_ECC_PLAIN_TEXT_384
                .ecc384_on_sce        = (uint32_t)&g_ecc384_on_sce,
            #else
  .ecc384_on_sce = 0,
#endif
#if R_SCE_SERVICES_ECC_WRAPPED_192
                .ecc192_on_sce_hrk    = (uint32_t)&g_ecc192_on_sce_hrk,
            #else
  .ecc192_on_sce_hrk = 0,
#endif
#if R_SCE_SERVICES_ECC_WRAPPED_224
                .ecc224_on_sce_hrk    = (uint32_t)&g_ecc224_on_sce_hrk,
            #else
  .ecc224_on_sce_hrk = 0,
#endif
#if R_SCE_SERVICES_ECC_WRAPPED_256
                .ecc256_on_sce_hrk    = (uint32_t)&g_ecc256_on_sce_hrk,
            #else
  .ecc256_on_sce_hrk = 0,
#endif
#if R_SCE_SERVICES_ECC_WRAPPED_384
                .ecc384_on_sce_hrk    = (uint32_t)&g_ecc384_on_sce_hrk,
            #else
  .ecc384_on_sce_hrk = 0,
#endif
#if R_SCE_SERVICES_TRNG
                .trng_on_sce          = (uint32_t)&g_trng_on_sce
            #else
  .trng_on_sce = 0
#endif
        };

#endif /* Crypto on S1 / SCE5 / SCE7 */

crypto_ctrl_t g_sce_ctrl;
crypto_cfg_t g_sce_cfg =
{ .p_sce_long_plg_start_callback = NULL, .p_sce_long_plg_end_callback = NULL, .endian_flag = CRYPTO_WORD_ENDIAN_LITTLE,
  .p_sce_api_interfaces = &g_sce_selected_api_interfaces };
const crypto_instance_t g_sce =
{ .p_ctrl = &g_sce_ctrl, .p_cfg = &g_sce_cfg, .p_api = &g_sce_crypto_api };

/** WEAK system error call back */
#if defined(__ICCARM__)
            #define g_sce_err_callback_WEAK_ATTRIBUTE
            #pragma weak g_sce_err_callback  = g_sce_err_callback_internal
            #elif defined(__GNUC__)
            #define g_sce_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_sce_err_callback_internal")))
            #endif
void g_sce_err_callback(void *p_instance, void *p_data)
g_sce_err_callback_WEAK_ATTRIBUTE;

/*******************************************************************************************************************//**
 * @brief        This is a weak example initialization error function.   
 *               It should be overridden by defining a user function with the prototype below.
 *               void g_sce_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]    p_instance arguments used to identify which instance caused the error and p_data Callback arguments 
 *               used to identify what error caused the callback. 
 **********************************************************************************************************************/
void g_sce_err_callback_internal(void *p_instance, void *p_data);
void g_sce_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}
trng_ctrl_t g_sce_trng_ctrl;
trng_cfg_t g_sce_trng_cfg =
{ .p_crypto_api = &g_sce_crypto_api, .nattempts = 2 };
const trng_instance_t g_sce_trng =
{ .p_ctrl = &g_sce_trng_ctrl, .p_cfg = &g_sce_trng_cfg, .p_api = &g_trng_on_sce };
/** Work memory to store Crypto HAL context for algorithm modules. */
static unsigned char g_sf_crypto0_memory_pool[1200 + 4] =
{ 0 };

/* Crypto Common Framework Control Block for g_sf_crypto0. */
sf_crypto_instance_ctrl_t g_sf_crypto0_ctrl;

/* Crypto Common Framework Configuration for g_sf_crypto0. */
const sf_crypto_cfg_t g_sf_crypto0_cfg =
{ .wait_option = TX_WAIT_FOREVER, .p_lower_lvl_crypto = (crypto_instance_t*) &g_sce, .p_extend = NULL,
  .p_context = NULL, .p_memory_pool = g_sf_crypto0_memory_pool, .memory_pool_size = 1200, .close_option =
          SF_CRYPTO_CLOSE_OPTION_DEFAULT, };

/* Crypto Common Framework Instance for g_sf_crypto0. */
const sf_crypto_instance_t g_sf_crypto0 =
{ .p_ctrl = &g_sf_crypto0_ctrl, .p_cfg = &g_sf_crypto0_cfg, .p_api = &g_sf_crypto_api };
NX_REC nx_record1;
static NX_CALLBACK_REC g_sf_el_nx_callbacks =
{ .nx_ether_unknown_packet_receive_callback = NULL, .nx_ether_mac_address_change_callback = NULL, };
static sf_el_nx_cfg_t sf_el_nx1_cfg =
{ .channel = 1, .nx_mac_address =
{ .nx_mac_address_h = SF_EL_NX_CFG_ENET1_MAC_H, .nx_mac_address_l = SF_EL_NX_CFG_ENET1_MAC_L },
  .p_callback_rec = &g_sf_el_nx_callbacks, .etherc_ptr = R_ETHERC1, .edmac_ptr = R_EDMAC1, };
#if SF_EL_NX_CFG_IRQ_IPL != BSP_IRQ_DISABLED
            #if !defined(SSP_SUPPRESS_ISR_g_sf_el_nx) && !defined(SSP_SUPPRESS_ISR_EDMAC1)
            SSP_VECTOR_DEFINE_CHAN(edmac_eint_isr, EDMAC, EINT, 1);
            #endif
            #endif

void nx_ether_driver_eth1(NX_IP_DRIVER *driver_req_ptr)
{
    nx_ether_driver (driver_req_ptr, &nx_record1, &sf_el_nx1_cfg);
}

/** Make user given name point to correct driver entry point. */
VOID (*g_sf_el_nx)(NX_IP_DRIVER *driver_req_ptr) = nx_ether_driver_eth1;
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void nx_common_init0(void)
 **********************************************************************************************************************/
void nx_common_init0(void)
{
    /** Initialize the NetX Duo system. */
    nx_system_initialize ();
}
NX_PACKET_POOL g_packet_pool0;
uint8_t g_packet_pool0_pool_memory[(96 * (1568 + sizeof(NX_PACKET)))];
#if defined(__ICCARM__)
            #define g_packet_pool0_err_callback_WEAK_ATTRIBUTE
            #pragma weak g_packet_pool0_err_callback  = g_packet_pool0_err_callback_internal
            #elif defined(__GNUC__)
            #define g_packet_pool0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_packet_pool0_err_callback_internal")))
            #endif
void g_packet_pool0_err_callback(void *p_instance, void *p_data)
g_packet_pool0_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_packet_pool0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_packet_pool0_err_callback_internal(void *p_instance, void *p_data);
void g_packet_pool0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void packet_pool_init0(void)
 **********************************************************************************************************************/
void packet_pool_init0(void)
{
    UINT g_packet_pool0_err;
    /* Create Client packet pool. */
    g_packet_pool0_err = nx_packet_pool_create (&g_packet_pool0, "g_packet_pool0 Packet Pool", 1568,
                                                &g_packet_pool0_pool_memory[0], (96 * (1568 + sizeof(NX_PACKET))));
    if (NX_SUCCESS != g_packet_pool0_err)
    {
        g_packet_pool0_err_callback ((void*) &g_packet_pool0, &g_packet_pool0_err);
    }
}
NX_IP g_ip0;
#ifndef NX_DISABLE_IPV6
UINT g_ip0_interface_index = 0;
UINT g_ip0_address_index;
NXD_ADDRESS g_ip0_global_ipv6_address;
NXD_ADDRESS g_ip0_local_ipv6_address;
#endif            
uint8_t g_ip0_stack_memory[2048] BSP_PLACE_IN_SECTION_V2(".stack.g_ip0") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
#if 1 == 1                       // Check for ARP is enabled
#if (0 == 0)    // Check for ARP cache storage units is in bytes
#define    NX_ARP_CACHE_SIZE    (520)
#else
#define    NX_ARP_CACHE_SIZE    (520 * sizeof(NX_ARP))
#endif
uint8_t g_ip0_arp_cache_memory[NX_ARP_CACHE_SIZE] BSP_ALIGN_VARIABLE(4);
#endif
ULONG g_ip0_actual_status;

#ifndef NULL
#define NULL_DEFINE
void NULL(struct NX_IP_STRUCT *ip_ptr, UINT interface_index, UINT link_up);
#endif            
#if defined(__ICCARM__)
            #define g_ip0_err_callback_WEAK_ATTRIBUTE
            #pragma weak g_ip0_err_callback  = g_ip0_err_callback_internal
            #elif defined(__GNUC__)
            #define g_ip0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_ip0_err_callback_internal")))
            #endif
void g_ip0_err_callback(void *p_instance, void *p_data)
g_ip0_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_ip0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_ip0_err_callback_internal(void *p_instance, void *p_data);
void g_ip0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void ip_init0(void)
 **********************************************************************************************************************/
void ip_init0(void)
{
    UINT g_ip0_err;
#ifndef NX_DISABLE_IPV6
    FILL_NXD_IPV6_ADDRESS(g_ip0_global_ipv6_address, 0x2001, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1);
    FILL_NXD_IPV6_ADDRESS(g_ip0_local_ipv6_address, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);

#endif
    /* Create an IP instance. */
    g_ip0_err = nx_ip_create (&g_ip0, "g_ip0 IP Instance", IP_ADDRESS (0, 0, 0, 0), IP_ADDRESS (255, 255, 255, 0),
                              &g_packet_pool0, g_sf_el_nx, &g_ip0_stack_memory[0], 2048, 3);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
#define SYNERGY_NOT_DEFINED     (0xFFFFFFFF)
#if (SYNERGY_NOT_DEFINED != 1)
    g_ip0_err = nx_arp_enable (&g_ip0, &g_ip0_arp_cache_memory[0], NX_ARP_CACHE_SIZE);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != SYNERGY_NOT_DEFINED)
                g_ip0_err = nx_rarp_enable(&g_ip0);
                if (NX_SUCCESS != g_ip0_err)
                {
                    g_ip0_err_callback((void *)&g_ip0,&g_ip0_err);
                }
                #endif
#if (SYNERGY_NOT_DEFINED != 1)
    g_ip0_err = nx_tcp_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != 1)
    g_ip0_err = nx_udp_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != 1)
    g_ip0_err = nx_icmp_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != 1)
    g_ip0_err = nx_igmp_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != SYNERGY_NOT_DEFINED)
                g_ip0_err = nx_ip_fragment_enable(&g_ip0);
                if (NX_SUCCESS != g_ip0_err)
                {
                    g_ip0_err_callback((void *)&g_ip0,&g_ip0_err);
                }                        
                #endif            
#undef SYNERGY_NOT_DEFINED

#ifndef NX_DISABLE_IPV6
    /** Here's where IPv6 is enabled. */
    g_ip0_err = nxd_ipv6_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
    g_ip0_err = nxd_icmp_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
    /* Wait for link to be initialized so MAC address is valid. */
    /** Wait for init to finish. */
    g_ip0_err = nx_ip_interface_status_check (&g_ip0, 0, NX_IP_INITIALIZE_DONE, &g_ip0_actual_status, NX_WAIT_FOREVER);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
    /** Setting link local address */
    if (0x0
            == (g_ip0_local_ipv6_address.nxd_ip_address.v6[0] | g_ip0_local_ipv6_address.nxd_ip_address.v6[1]
                    | g_ip0_local_ipv6_address.nxd_ip_address.v6[2] | g_ip0_local_ipv6_address.nxd_ip_address.v6[3]))
    {
        g_ip0_err = nxd_ipv6_address_set (&g_ip0, g_ip0_interface_index, NX_NULL, 10, NX_NULL);
    }
    else
    {
        g_ip0_err = nxd_ipv6_address_set (&g_ip0, g_ip0_interface_index, &g_ip0_local_ipv6_address, 10,
                                          &g_ip0_address_index);
    }
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
    if (0x0
            != (g_ip0_global_ipv6_address.nxd_ip_address.v6[0] | g_ip0_global_ipv6_address.nxd_ip_address.v6[1]
                    | g_ip0_global_ipv6_address.nxd_ip_address.v6[2] | g_ip0_global_ipv6_address.nxd_ip_address.v6[3]))
    {
        g_ip0_err = nxd_ipv6_address_set (&g_ip0, g_ip0_interface_index, &g_ip0_global_ipv6_address, 64,
                                          &g_ip0_address_index);
    }
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
#endif

#ifdef NULL_DEFINE
    g_ip0_err = nx_ip_link_status_change_notify_set (&g_ip0, NULL);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void*) &g_ip0, &g_ip0_err);
    }
#endif

    /* Gateway IP Address */
#define IP_VALID(a,b,c,d)     (a|b|c|d)
#if IP_VALID(0,0,0,0)
                g_ip0_err = nx_ip_gateway_address_set(&g_ip0,
											     IP_ADDRESS(0,0,0,0));       
														   
                if (NX_SUCCESS != g_ip0_err)
                {
                    g_ip0_err_callback((void *)&g_ip0,&g_ip0_err);
                }                       
                #endif         
#undef IP_VALID

}
#define FX_COMMON_INITIALIZE (1)
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void fx_common_init0(void)
 **********************************************************************************************************************/
void fx_common_init0(void)
{
    /** Initialize the FileX system. */
    fx_system_initialize ();
}
#if defined(__ICCARM__)
        #define ux_v2_err_callback_WEAK_ATTRIBUTE
        #pragma weak ux_v2_err_callback  = ux_v2_err_callback_internal
        #elif defined(__GNUC__)
        #define ux_v2_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("ux_v2_err_callback_internal")))
        #endif
void ux_v2_err_callback(void *p_instance, void *p_data)
ux_v2_err_callback_WEAK_ATTRIBUTE;
/* memory pool allocation used by USBX system. */
CHAR g_ux_pool_memory[63488] =
{ 0 };
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function 
 *             with the prototype below.
 *             - void ux_v2_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void ux_v2_err_callback_internal(void *p_instance, void *p_data);
void ux_v2_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

#ifdef UX_HOST_INITIALIZE

#ifdef USB_HOST_STORAGE_CLASS_REGISTER

/* A semaphore for synchronizing to the USB media storage mount timing. */
static TX_SEMAPHORE ux_host_storage_semaphore_insertion;

/* Temporary storage place for the pointer to a USBX Host Mass Storage Class Instance. */
static UX_HOST_CLASS_STORAGE *g_ux_new_host_storage_instance;

/* Function prototype for the function to notify changes happened to the USBX Host Class Mass Storage. */
static UINT ux_system_host_storage_change_function(ULONG event, VOID *instance);

/*******************************************************************************************************************//**
 * @brief      This is the function to register the USBX Host Class Mass Storage.
 **********************************************************************************************************************/
void ux_host_stack_class_register_storage(void)
{
    UINT status;
    status = ux_host_stack_class_register (_ux_system_host_class_storage_name, ux_host_class_storage_entry);
    if (UX_SUCCESS != status)
    {
        ux_v2_err_callback (NULL, &status);
    }

    /* Create semaphores used for the USB Mass Storage Media Initialization. */

    if (ux_host_storage_semaphore_insertion.tx_semaphore_id != ((ULONG) 0x53454D41))
    {
        status = tx_semaphore_create (&ux_host_storage_semaphore_insertion, "ux_host_storage_semaphore_insertion", 0);
        if (TX_SUCCESS != status)
        {
            ux_v2_err_callback (NULL, &status);
        }
    }
}

/*******************************************************************************************************************//**
 * @brief      This is the function to notify changes happened to the USBX Host Class Mass Storage.
 * @param[in]  event      Event code from happened to the USBX Host system.
 * @param[in]  instance   Pointer to a USBX Host class instance, which occurs a event.
 * @retval     USBX error code
 **********************************************************************************************************************/
static UINT ux_system_host_storage_change_function(ULONG event, VOID *instance)
{
    if (UX_DEVICE_INSERTION == event) /* Check if there is a device insertion. */
    {
        g_ux_new_host_storage_instance = instance;

        /* Put the semaphore for a USBX Mass Storage Media insertion. */
        tx_semaphore_put (&ux_host_storage_semaphore_insertion);
    }
    else if (UX_DEVICE_REMOVAL == event) /* Check if there is a device removal. */
    {
        g_ux_new_host_storage_instance = NULL;
    }
    return UX_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      This is the function to get the FileX Media Control Block for a USB Mass Storage device.
 * @param[in]      new_instance     Pointer to a USBX Host Mass Storage Class instance.
 * @param[in/out]  p_storage_media  Pointer of the pointer to save an instance of the USBX Host Mass Storage Media.
 * @param[in/out]  p_fx_media       Pointer of the pointer to save an instance of FileX Media Control Block.
 * @retval     UX_HOST_CLASS_INSTANCE_UNKNOWN  The instance provided was not for a valid Mass Storage device.
 **********************************************************************************************************************/
UINT ux_system_host_storage_fx_media_get(UX_HOST_CLASS_STORAGE *instance, UX_HOST_CLASS_STORAGE_MEDIA **p_storage_media,
        FX_MEDIA **p_fx_media)
{
    UINT error = UX_SUCCESS;
    UX_HOST_CLASS *p_host_class;
    UX_HOST_CLASS_STORAGE_MEDIA *p_storage_media_local;
    FX_MEDIA *p_fx_media_local;
    int index;

    /** Get the USBX Host Mass Storage Class. */
    ux_host_stack_class_get (_ux_system_host_class_storage_name, &p_host_class);

    /** Get the pointer to a media attached to the class container for USB Host Mass Storage. */
    p_storage_media_local = (UX_HOST_CLASS_STORAGE_MEDIA*) p_host_class->ux_host_class_media;

    /** Get the pointer to a FileX Media Control Block. */
    for (index = 0; index < UX_HOST_CLASS_STORAGE_MAX_MEDIA; index++)
    {
        p_fx_media_local = &p_storage_media_local->ux_host_class_storage_media;
        if (p_fx_media_local->fx_media_driver_info != instance)
        {
            /* None of FileX Media Control Blocks matched to this USBX Host Mass Storage Instance. */
            p_storage_media_local++;
        }
        else
        {
            /* Found a FileX Media Control Block used for this USBX Host Mass Storage Instance. */
            break;
        }
    }
    if (UX_HOST_CLASS_STORAGE_MAX_MEDIA == index)
    {
        error = UX_HOST_CLASS_INSTANCE_UNKNOWN;
    }
    else
    {
        *p_storage_media = p_storage_media_local;
        *p_fx_media = p_fx_media_local;
    }

    return error;
}
#endif /* USB_HOST_STORAGE_CLASS_REGISTER */

/*******************************************************************************************************************//**
 * @brief      This is the function to notify a USB event from the USBX Host system.
 * @param[in]  event     Event code from happened to the USBX Host system.
 * @param[in]  usb_class Pointer to a USBX Host class, which occurs a event.
 * @param[in]  instance  Pointer to a USBX Host class instance, which occurs a event.
 * @retval     USBX error code
 **********************************************************************************************************************/
UINT ux_system_host_change_function(ULONG event, UX_HOST_CLASS *host_class, VOID *instance)
{
    UINT status = UX_SUCCESS;
    SSP_PARAMETER_NOT_USED (event);
    SSP_PARAMETER_NOT_USED (host_class);
    SSP_PARAMETER_NOT_USED (instance);

#if !defined(usb_host_plug_event_notification)
    /* Call user function back for USBX Host Class event notification. */
    status = usb_host_plug_event_notification (event, host_class, instance);
    if (UX_SUCCESS != status)
    {
        return status;
    }
#endif

#ifdef USB_HOST_STORAGE_CLASS_REGISTER
    /* Check the class container if it is for a USBX Host Mass Storage class. */
    if (UX_SUCCESS
            == _ux_utility_memory_compare (_ux_system_host_class_storage_name, host_class,
                                           _ux_utility_string_length_get (_ux_system_host_class_storage_name)))
    {
        status = ux_system_host_storage_change_function (event, instance);
    }
#endif
    return status;
}
#endif

#ifdef USB_HOST_HID_CLASS_REGISTER
            /* Function prototype to register USBX Host HID Clients to the USBX Host system. */
            static void ux_host_class_hid_clients_register(void);
#endif

/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void ux_common_init0(void)
 **********************************************************************************************************************/
void ux_common_init0(void)
{
    UINT status_ux_init;

    /** Initialize the USBX system. */
    status_ux_init = ux_system_initialize ((CHAR*) g_ux_pool_memory, 63488, UX_NULL, 0);
    if (UX_SUCCESS != status_ux_init)
    {
        ux_v2_err_callback (NULL, &status_ux_init);
    }

#ifdef UX_HOST_INITIALIZE
    /** Initialize the USBX Host stack. */
    status_ux_init = ux_host_stack_initialize (ux_system_host_change_function);
    if (UX_SUCCESS != status_ux_init)
    {
        ux_v2_err_callback (NULL, &status_ux_init);
    }

    /** Register USB Host classes. */
#ifdef USB_HOST_HUB_CLASS_REGISTER
                status_ux_init =  ux_host_stack_class_register(_ux_system_host_class_hub_name, ux_host_class_hub_entry);
                if (UX_SUCCESS != status_ux_init)
                {
                    ux_v2_err_callback(NULL,&status_ux_init);
                }
#endif

#ifdef USB_HOST_CDC_ACM_CLASS_REGISTER
                status_ux_init =  ux_host_stack_class_register(_ux_system_host_class_cdc_acm_name, ux_host_class_cdc_acm_entry);
                if (UX_SUCCESS != status_ux_init)
                {
                    ux_v2_err_callback(NULL,&status_ux_init);
                }
#endif

#ifdef USB_HOST_HID_CLASS_REGISTER
                status_ux_init =  ux_host_stack_class_register(_ux_system_host_class_hid_name, ux_host_class_hid_entry);
                if (UX_SUCCESS != status_ux_init)
                {
                    ux_v2_err_callback(NULL,&status_ux_init);
                }
            
                ux_host_class_hid_clients_register ();
#endif

#ifdef USB_HOST_STORAGE_CLASS_REGISTER
    ux_host_stack_class_register_storage ();
#endif

#ifdef USB_HOST_VIDEO_CLASS_REGISTER
                status_ux_init =  ux_host_stack_class_register(_ux_system_host_class_video_name, ux_host_class_video_entry);
                if (UX_SUCCESS != status_ux_init)
                {
                    ux_v2_err_callback(NULL,&status_ux_init);
                }
#endif

#ifdef USB_HOST_AUDIO_CLASS_REGISTER
                status_ux_init =  ux_host_stack_class_register(_ux_system_host_class_audio_name, ux_host_class_audio_entry);
                if (UX_SUCCESS != status_ux_init)
                {
                    ux_v2_err_callback(NULL,&status_ux_init);
                }
#endif

#ifdef USB_HOST_PRINTER_CLASS_REGISTER
                status_ux_init =  ux_host_stack_class_register(_ux_system_host_class_printer_name, _ux_host_class_printer_entry);
                if (UX_SUCCESS != status_ux_init)
                {
                    ux_v2_err_callback(NULL,&status_ux_init);
                }
#endif

#endif /* UX_HOST_INITIALIZE */
}
#if (2) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer3) && !defined(SSP_SUPPRESS_ISR_DMACELC_EVENT_ELC_SOFTWARE_EVENT_1)
SSP_VECTOR_DEFINE_CHAN(dmac_int_isr, DMAC, INT, 1);
#endif
#endif
dmac_instance_ctrl_t g_transfer3_ctrl;
transfer_info_t g_transfer3_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .repeat_area = TRANSFER_REPEAT_AREA_DESTINATION, .irq =
          TRANSFER_IRQ_EACH,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED, .src_addr_mode = TRANSFER_ADDR_MODE_FIXED, .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_BLOCK, .p_dest = (void*) NULL, .p_src = (void const*) NULL, .num_blocks = 0, .length = 0, };
const transfer_on_dmac_cfg_t g_transfer3_extend =
{ .channel = 1, .offset_byte = 0, };
const transfer_cfg_t g_transfer3_cfg =
{ .p_info = &g_transfer3_info, .activation_source = ELC_EVENT_ELC_SOFTWARE_EVENT_1, .auto_enable = false, .p_callback =
          NULL,
  .p_context = &g_transfer3, .irq_ipl = (2), .p_extend = &g_transfer3_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer3 =
{ .p_ctrl = &g_transfer3_ctrl, .p_cfg = &g_transfer3_cfg, .p_api = &g_transfer_on_dmac };
#if (2) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer2) && !defined(SSP_SUPPRESS_ISR_DMACELC_EVENT_ELC_SOFTWARE_EVENT_0)
SSP_VECTOR_DEFINE_CHAN(dmac_int_isr, DMAC, INT, 0);
#endif
#endif
dmac_instance_ctrl_t g_transfer2_ctrl;
transfer_info_t g_transfer2_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED, .repeat_area = TRANSFER_REPEAT_AREA_SOURCE, .irq = TRANSFER_IRQ_EACH,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED, .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .size =
          TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_BLOCK, .p_dest = (void*) NULL, .p_src = (void const*) NULL, .num_blocks = 0, .length = 0, };
const transfer_on_dmac_cfg_t g_transfer2_extend =
{ .channel = 0, .offset_byte = 0, };
const transfer_cfg_t g_transfer2_cfg =
{ .p_info = &g_transfer2_info, .activation_source = ELC_EVENT_ELC_SOFTWARE_EVENT_0, .auto_enable = false, .p_callback =
          NULL,
  .p_context = &g_transfer2, .irq_ipl = (2), .p_extend = &g_transfer2_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer2 =
{ .p_ctrl = &g_transfer2_ctrl, .p_cfg = &g_transfer2_cfg, .p_api = &g_transfer_on_dmac };
/***********************************************************************************************************************
 * Registers Interrupt Vector for USBHS Controller.
 **********************************************************************************************************************/
#if (SF_EL_UX_HCD_CFG_HS_IRQ_IPL != BSP_IRQ_DISABLED)
            /* USBHS ISR vector registering. */
            #if !defined(SSP_SUPPRESS_ISR_g_sf_el_ux_hcd_hs_0) && !defined(SSP_SUPPRESS_ISR_USB)
            #if !defined(UX_DCD_SYNERGY_USBHS_VECTOR)
            SSP_VECTOR_DEFINE_UNIT(usbhs_usb_int_resume_isr, USB, HS, USB_INT_RESUME, 0);
            #endif
            #endif
            #endif

#undef SYNERGY_NOT_DEFINED
#define SYNERGY_NOT_DEFINED (1)
/***********************************************************************************************************************
 * The definition of wrapper interface for USBX Synergy Port HCD Driver.
 **********************************************************************************************************************/
static UINT g_sf_el_ux_hcd_hs_0_initialize(UX_HCD *hcd)
{
#if ((SYNERGY_NOT_DEFINED != g_transfer2) && (SYNERGY_NOT_DEFINED != g_transfer3))
    /* DMA support */
    UX_HCD_SYNERGY_TRANSFER hcd_transfer;
    hcd_transfer.ux_synergy_transfer_tx = (transfer_instance_t*) &g_transfer2;
    hcd_transfer.ux_synergy_transfer_rx = (transfer_instance_t*) &g_transfer3;
    return (UINT) ux_hcd_synergy_initialize_transfer_support (hcd, (UX_HCD_SYNERGY_TRANSFER*) &hcd_transfer);
#else
                /* Non DMA support */
                return (UINT)ux_hcd_synergy_initialize(hcd);
            #endif
} /* End of function g_sf_el_ux_hcd_hs_0_initialize() */
#undef SYNERGY_NOT_DEFINED
#if defined(__ICCARM__)
              #define g_ux_host_0_err_callback_WEAK_ATTRIBUTE
              #pragma weak g_ux_host_0_err_callback  = g_ux_host_0_err_callback_internal
              #elif defined(__GNUC__)
              #define g_ux_host_0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_ux_host_0_err_callback_internal")))
              #endif
void g_ux_host_0_err_callback(void *p_instance, void *p_data)
g_ux_host_0_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function 
 *             with the prototype below.
 *             - void g_ux_host_0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_ux_host_0_err_callback_internal(void *p_instance, void *p_data);
void g_ux_host_0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void ux_host_init0(void)
 **********************************************************************************************************************/
void ux_host_init0(void)
{
    /** Register a USB host controller. */
    UINT status_g_ux_host_0 = ux_host_stack_hcd_register ((UCHAR*) "g_sf_el_ux_hcd_hs_0",
                                                          g_sf_el_ux_hcd_hs_0_initialize, R_USBHS_BASE,
                                                          UX_SYNERGY_CONTROLLER);
    if (UX_SUCCESS != status_g_ux_host_0)
    {
        g_ux_host_0_err_callback (NULL, &status_g_ux_host_0);
    }
}
/* Pointer to a USBX Host Mass Storage Class Instance */
UX_HOST_CLASS_STORAGE *g_ux_host_class_storage0;
/* A pointer to FileX Media Control Block for a USB flash device. */
FX_MEDIA *g_fx_media0_ptr;

/* Pointer to a USBX Host Mass Storage Class Media. */
UX_HOST_CLASS_STORAGE_MEDIA *g_ux_host_class_storage0_media;

#if defined(__ICCARM__)
            #define g_fx_media0_err_callback_failed_to_get_fx_media_WEAK_ATTRIBUTE
            #pragma weak g_fx_media0_err_callback_failed_to_get_fx_media  = g_fx_media0_err_callback_failed_to_get_fx_media_internal
            #elif defined(__GNUC__)
            #define g_fx_media0_err_callback_failed_to_get_fx_media_WEAK_ATTRIBUTE __attribute__ ((weak, alias("g_fx_media0_err_callback_failed_to_get_fx_media_internal")))
            #endif
void g_fx_media0_err_callback_failed_to_get_fx_media(void *p_instance, void *p_data)
g_fx_media0_err_callback_failed_to_get_fx_media_WEAK_ATTRIBUTE;

/* Function prototype for the weak initialization error function. */
void g_fx_media0_err_callback_failed_to_get_fx_media_internal(void *p_instance, void *p_data);

/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user function
 *             with the prototype below.
 *             - void g_fx_media0_err_callback_failed_to_get_fx_media(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance  Not used.
 * @param[in]  p_data      Not used.
 **********************************************************************************************************************/
void g_fx_media0_err_callback_failed_to_get_fx_media_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** A FileX media control block for a USB mass storage device was not found. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

/*******************************************************************************************************************//**
 * @brief      This is the function to get g_fx_media0 for a USB Mass Storage device.
 **********************************************************************************************************************/
void fx_media_init_function0(void)
{
    /** Suspend the thread until a USB Mass Storage Device inserted. */
    UINT err_g_fx_media0_semaphore_get = tx_semaphore_get (&ux_host_storage_semaphore_insertion, 1000);
    if (TX_SUCCESS != err_g_fx_media0_semaphore_get)
    {
        g_fx_media0_err_callback_failed_to_get_fx_media (&ux_host_storage_semaphore_insertion,
                                                         &err_g_fx_media0_semaphore_get);
    }

    /** Initialize the FileX Media Control Block g_fx_media0 for a USB Mass Storage device. */
    /** Get the pointer to a USBX Host Mass Storage Instance. */
    g_ux_host_class_storage0 = g_ux_new_host_storage_instance;

    /** Initialize the FileX Media Control Block g_fx_media0 for a USB Mass Storage device. */
    UINT err_g_fx_media0_get = ux_system_host_storage_fx_media_get (g_ux_new_host_storage_instance,
                                                                    &g_ux_host_class_storage0_media, &g_fx_media0_ptr);
    if (UX_SUCCESS != err_g_fx_media0_get)
    {
        g_fx_media0_err_callback_failed_to_get_fx_media (&g_fx_media0_ptr, &err_g_fx_media0_get);
    }
}
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer5) && !defined(SSP_SUPPRESS_ISR_DTCELC_EVENT_IIC2_RXI)
#define DTC_ACTIVATION_SRC_ELC_EVENT_IIC2_RXI
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_0) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_0);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0
#endif
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_1) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_1);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1
#endif
#endif
#endif

dtc_instance_ctrl_t g_transfer5_ctrl;
transfer_info_t g_transfer5_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .repeat_area = TRANSFER_REPEAT_AREA_DESTINATION, .irq =
          TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED, .src_addr_mode = TRANSFER_ADDR_MODE_FIXED, .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL, .p_dest = (void*) NULL, .p_src = (void const*) NULL, .num_blocks = 0, .length = 0, };
const transfer_cfg_t g_transfer5_cfg =
{ .p_info = &g_transfer5_info, .activation_source = ELC_EVENT_IIC2_RXI, .auto_enable = false, .p_callback = NULL,
  .p_context = &g_transfer5, .irq_ipl = (BSP_IRQ_DISABLED) };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer5 =
{ .p_ctrl = &g_transfer5_ctrl, .p_cfg = &g_transfer5_cfg, .p_api = &g_transfer_on_dtc };
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer4) && !defined(SSP_SUPPRESS_ISR_DTCELC_EVENT_IIC2_TXI)
#define DTC_ACTIVATION_SRC_ELC_EVENT_IIC2_TXI
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_0) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_0);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_0
#endif
#if defined(DTC_ACTIVATION_SRC_ELC_EVENT_ELC_SOFTWARE_EVENT_1) && !defined(DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1)
SSP_VECTOR_DEFINE(elc_software_event_isr, ELC, SOFTWARE_EVENT_1);
#define DTC_VECTOR_DEFINED_SOFTWARE_EVENT_1
#endif
#endif
#endif

dtc_instance_ctrl_t g_transfer4_ctrl;
transfer_info_t g_transfer4_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED, .repeat_area = TRANSFER_REPEAT_AREA_SOURCE, .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED, .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .size =
          TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL, .p_dest = (void*) NULL, .p_src = (void const*) NULL, .num_blocks = 0, .length = 0, };
const transfer_cfg_t g_transfer4_cfg =
{ .p_info = &g_transfer4_info, .activation_source = ELC_EVENT_IIC2_TXI, .auto_enable = false, .p_callback = NULL,
  .p_context = &g_transfer4, .irq_ipl = (BSP_IRQ_DISABLED) };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer4 =
{ .p_ctrl = &g_transfer4_ctrl, .p_cfg = &g_transfer4_cfg, .p_api = &g_transfer_on_dtc };
#if !defined(SSP_SUPPRESS_ISR_g_i2c) && !defined(SSP_SUPPRESS_ISR_IIC2)
SSP_VECTOR_DEFINE_CHAN(iic_rxi_isr, IIC, RXI, 2);
#endif
#if !defined(SSP_SUPPRESS_ISR_g_i2c) && !defined(SSP_SUPPRESS_ISR_IIC2)
SSP_VECTOR_DEFINE_CHAN(iic_txi_isr, IIC, TXI, 2);
#endif
#if !defined(SSP_SUPPRESS_ISR_g_i2c) && !defined(SSP_SUPPRESS_ISR_IIC2)
SSP_VECTOR_DEFINE_CHAN(iic_tei_isr, IIC, TEI, 2);
#endif
#if !defined(SSP_SUPPRESS_ISR_g_i2c) && !defined(SSP_SUPPRESS_ISR_IIC2)
SSP_VECTOR_DEFINE_CHAN(iic_eri_isr, IIC, ERI, 2);
#endif
riic_instance_ctrl_t g_i2c_ctrl;
const riic_extended_cfg g_i2c_extend =
{ .timeout_mode = RIIC_TIMEOUT_MODE_SHORT, };
const i2c_cfg_t g_i2c_cfg =
        { .channel = 2, .rate = I2C_RATE_FAST, .slave = 0, .addr_mode = I2C_ADDR_MODE_7BIT, .sda_delay = 0,
#define SYNERGY_NOT_DEFINED (1)            
#if (SYNERGY_NOT_DEFINED == g_transfer4)
                .p_transfer_tx       = NULL,
#else
          .p_transfer_tx = &g_transfer4,
#endif
#if (SYNERGY_NOT_DEFINED == g_transfer5)
                .p_transfer_rx       = NULL,
#else
          .p_transfer_rx = &g_transfer5,
#endif
#undef SYNERGY_NOT_DEFINED	
          .p_callback = NULL,
          .p_context = (void*) &g_i2c, .rxi_ipl = (3), .txi_ipl = (3), .tei_ipl = (3), .eri_ipl = (3), .p_extend =
                  &g_i2c_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c =
{ .p_ctrl = &g_i2c_ctrl, .p_cfg = &g_i2c_cfg, .p_api = &g_i2c_master_on_riic };
static TX_MUTEX sf_bus_mutex_g_sf_i2c_bus0;
static TX_EVENT_FLAGS_GROUP sf_bus_eventflag_g_sf_i2c_bus0;
static sf_i2c_instance_ctrl_t *sf_curr_ctrl_g_sf_i2c_bus0;
static sf_i2c_instance_ctrl_t *sf_curr_bus_ctrl_g_sf_i2c_bus0;
sf_i2c_bus_t g_sf_i2c_bus0 =
{ .p_bus_name = (uint8_t*) "g_sf_i2c_bus0", .channel = 2, .p_lock_mutex = &sf_bus_mutex_g_sf_i2c_bus0,
  .p_sync_eventflag = &sf_bus_eventflag_g_sf_i2c_bus0, .pp_curr_ctrl = (sf_i2c_ctrl_t**) &sf_curr_ctrl_g_sf_i2c_bus0,
  .p_lower_lvl_api = (i2c_api_master_t*) &g_i2c_master_on_riic, .device_count = 0, .pp_curr_bus_ctrl =
          (sf_i2c_ctrl_t**) &sf_curr_bus_ctrl_g_sf_i2c_bus0, };
#if SYNERGY_NOT_DEFINED != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_DRW)
SSP_VECTOR_DEFINE(drw_int_isr, DRW, INT);
#endif
#endif
SSP_VECTOR_DEFINE( jpeg_jdti_isr, JPEG, JDTI);
SSP_VECTOR_DEFINE( jpeg_jedi_isr, JPEG, JEDI);
static jpeg_decode_instance_ctrl_t g_jpeg_decode0_ctrl;
const jpeg_decode_cfg_t g_jpeg_decode0_cfg =
{ .input_data_format = JPEG_DECODE_DATA_FORMAT_NORMAL, .output_data_format = JPEG_DECODE_DATA_FORMAT_NORMAL,
  .pixel_format = JPEG_DECODE_PIXEL_FORMAT_RGB565, .alpha_value = 255, .p_callback = NULL, .jedi_ipl = (3), .jdti_ipl =
          (3), };
const jpeg_decode_instance_t g_jpeg_decode0 =
{ .p_api = (jpeg_decode_api_t const*) &g_jpeg_decode_on_jpeg_decode, .p_ctrl = &g_jpeg_decode0_ctrl, .p_cfg =
          &g_jpeg_decode0_cfg };
static sf_jpeg_decode_instance_ctrl_t g_sf_jpeg_decode0_ctrl;

static const sf_jpeg_decode_cfg_t g_sf_jpeg_decode0_cfg =
{ .p_lower_lvl_jpeg_decode = (jpeg_decode_instance_t const*) &g_jpeg_decode0 };
const sf_jpeg_decode_instance_t g_sf_jpeg_decode0 =
{ .p_api = &g_sf_jpeg_decode_on_sf_jpeg_decode, .p_ctrl = &g_sf_jpeg_decode0_ctrl, .p_cfg =
          (sf_jpeg_decode_cfg_t const*) &g_sf_jpeg_decode0_cfg };
#if (3) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_display) && !defined(SSP_SUPPRESS_ISR_GLCD)
SSP_VECTOR_DEFINE( glcdc_line_detect_isr, GLCDC, LINE_DETECT);
#endif
#endif
#if (3) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_display) && !defined(SSP_SUPPRESS_ISR_GLCD)
SSP_VECTOR_DEFINE( glcdc_underflow_1_isr, GLCDC, UNDERFLOW_1);
#endif
#endif
#if (3) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_display) && !defined(SSP_SUPPRESS_ISR_GLCD)
SSP_VECTOR_DEFINE( glcdc_underflow_2_isr, GLCDC, UNDERFLOW_2);
#endif
#endif

/** Display frame buffer */
#if (true)
        uint8_t g_display_fb_background[1][((256 * 320) * DISPLAY_BITS_PER_PIXEL_INPUT0) >> 3] BSP_ALIGN_VARIABLE_V2(64) BSP_PLACE_IN_SECTION_V2(".bss");
        #else
/** Graphics screen1 is specified not to be used when starting */
#endif
#if (false)
        uint8_t g_display_fb_foreground[2][((800 * 480) * DISPLAY_BITS_PER_PIXEL_INPUT1) >> 3] BSP_ALIGN_VARIABLE_V2(64) BSP_PLACE_IN_SECTION_V2(".sdram");
        #else
/** Graphics screen2 is specified not to be used when starting */
#endif

#if (false)
        /** Display CLUT buffer to be used for updating CLUT */
        static uint32_t CLUT_buffer[256];

        /** Display CLUT configuration(only used if using CLUT format) */
        display_clut_cfg_t g_display_clut_cfg_glcd =
        {
            .p_base              = (uint32_t *)CLUT_buffer,
            .start               = 0,   /* User have to update this setting when using */
            .size                = 256  /* User have to update this setting when using */
        };
        #else
/** CLUT is specified not to be used */
#endif

#if (false | false | false)
        /** Display interface configuration */
        static const display_gamma_correction_t g_display_gamma_cfg =
        {
            .r =
            {
                .enable      = false,
                .gain        = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                .threshold   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
            },
            .g =
            {
                .enable      = false,
                .gain        = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                .threshold   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
            },
            .b =
            {
                .enable      = false,
                .gain        = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                .threshold   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
            }
        };
        #endif

/** Display device extended configuration */
static const glcd_cfg_t g_display_extend_cfg =
{ .tcon_hsync = GLCD_TCON_PIN_2, .tcon_vsync = GLCD_TCON_PIN_1, .tcon_de = GLCD_TCON_PIN_0, .correction_proc_order =
          GLCD_CORRECTION_PROC_ORDER_BRIGHTNESS_CONTRAST2GAMMA,
  .clksrc = GLCD_CLK_SRC_INTERNAL, .clock_div_ratio = GLCD_PANEL_CLK_DIVISOR_32, .dithering_mode =
          GLCD_DITHERING_MODE_TRUNCATE,
  .dithering_pattern_A = GLCD_DITHERING_PATTERN_11, .dithering_pattern_B = GLCD_DITHERING_PATTERN_11,
  .dithering_pattern_C = GLCD_DITHERING_PATTERN_11, .dithering_pattern_D = GLCD_DITHERING_PATTERN_11 };

/** Display control block instance */
glcd_instance_ctrl_t g_display_ctrl;

/** Display interface configuration */
const display_cfg_t g_display_cfg =
{
/** Input1(Graphics1 screen) configuration */
.input[0] =
{
#if (true)
                .p_base              = (uint32_t *)&g_display_fb_background[0],
                #else
  .p_base = NULL,
#endif
  .hsize = 256,
  .vsize = 320, .hstride = 256, .format = DISPLAY_IN_FORMAT_16BITS_RGB565, .line_descending_enable = false,
  .lines_repeat_enable = false, .lines_repeat_times = 0 },

  /** Input2(Graphics2 screen) configuration */
  .input[1] =
  {
#if (false)
                .p_base              = (uint32_t *)&g_display_fb_foreground[0],
                #else
    .p_base = NULL,
#endif
    .hsize = 800,
    .vsize = 480, .hstride = 800, .format = DISPLAY_IN_FORMAT_16BITS_RGB565, .line_descending_enable = false,
    .lines_repeat_enable = false, .lines_repeat_times = 0 },

  /** Input1(Graphics1 screen) layer configuration */
  .layer[0] =
  { .coordinate =
  { .x = 0, .y = 0 },
    .bg_color =
    { .byte =
    { .a = 255, .r = 255, .g = 255, .b = 255 } },
    .fade_control = DISPLAY_FADE_CONTROL_NONE, .fade_speed = 0 },

  /** Input2(Graphics2 screen) layer configuration */
  .layer[1] =
  { .coordinate =
  { .x = 0, .y = 0 },
    .bg_color =
    { .byte =
    { .a = 255, .r = 255, .g = 255, .b = 255 } },
    .fade_control = DISPLAY_FADE_CONTROL_NONE, .fade_speed = 0 },

  /** Output configuration */
  .output =
  { .htiming =
  { .total_cyc = 320, .display_cyc = 240, .back_porch = 6, .sync_width = 4, .sync_polarity =
            DISPLAY_SIGNAL_POLARITY_LOACTIVE },
    .vtiming =
    { .total_cyc = 328, .display_cyc = 320, .back_porch = 4, .sync_width = 4, .sync_polarity =
              DISPLAY_SIGNAL_POLARITY_LOACTIVE },
    .format = DISPLAY_OUT_FORMAT_16BITS_RGB565, .endian = DISPLAY_ENDIAN_LITTLE, .color_order = DISPLAY_COLOR_ORDER_RGB,
    .data_enable_polarity = DISPLAY_SIGNAL_POLARITY_HIACTIVE, .sync_edge = DISPLAY_SIGNAL_SYNC_EDGE_RISING, .bg_color =
    { .byte =
    { .a = 255, .r = 0, .g = 0, .b = 0 } },
    .brightness =
    { .enable = false, .r = 512, .g = 512, .b = 512 },
    .contrast =
    { .enable = false, .r = 128, .g = 128, .b = 128 },
#if (false | false | false)
                .p_gamma_correction  = (display_gamma_correction_t *)(&g_display_gamma_cfg),
#else
    .p_gamma_correction = NULL,
#endif
    .dithering_on = false },

  /** Display device callback function pointer */
  .p_callback = NULL,
  .p_context = (void*) &g_display,

  /** Display device extended configuration */
  .p_extend = (void*) (&g_display_extend_cfg),

  .line_detect_ipl = (3),
  .underflow_1_ipl = (3), .underflow_2_ipl = (3), };

#if (true)
        /** Display on GLCD run-time configuration(for the graphics1 screen) */
        display_runtime_cfg_t g_display_runtime_cfg_bg =
        {
            .input =
            {
                #if (true)
                .p_base              = (uint32_t *)&g_display_fb_background[0],
                #else
                .p_base              = NULL,
                #endif
                .hsize               = 256,
                .vsize               = 320,
                .hstride             = 256,
                .format              = DISPLAY_IN_FORMAT_16BITS_RGB565,
                .line_descending_enable = false,
                .lines_repeat_enable = false,
                .lines_repeat_times  = 0
            },
            .layer =
            {
                .coordinate = {
                        .x           = 0,
                        .y           = 0
                },
                .bg_color            =
                {
                    .byte            =
                    {
                        .a           = 255,
                        .r           = 255,
                        .g           = 255,
                        .b           = 255
                    }
                },
                .fade_control        = DISPLAY_FADE_CONTROL_NONE,
                .fade_speed          = 0
            }
        };
#endif
#if (false)
        /** Display on GLCD run-time configuration(for the graphics2 screen) */
        display_runtime_cfg_t g_display_runtime_cfg_fg =
        {
            .input =
            {
                #if (false)
                .p_base              = (uint32_t *)&g_display_fb_foreground[0],
                #else
                .p_base              = NULL,
                #endif
                .hsize               = 800,
                .vsize               = 480,
                .hstride             = 800,
                .format              = DISPLAY_IN_FORMAT_16BITS_RGB565,
                .line_descending_enable = false,
                .lines_repeat_enable = false,
                .lines_repeat_times  = 0
             },
            .layer =
            {
                .coordinate = {
                        .x           = 0,
                        .y           = 0
                },
                .bg_color            =
                {
                    .byte            =
                    {
                        .a           = 255,
                        .r           = 255,
                        .g           = 255,
                        .b           = 255
                    }
                },
                .fade_control        = DISPLAY_FADE_CONTROL_NONE,
                .fade_speed          = 0
            }
        };
#endif

/* Instance structure to use this module. */
const display_instance_t g_display =
{ .p_ctrl = &g_display_ctrl, .p_cfg = (display_cfg_t*) &g_display_cfg, .p_api = (display_api_t*) &g_display_on_glcd };
/** GUIX Canvas Buffer */
#if false
            #if (1 == 1) /* Inherit Frame Buffer Name from Graphics Screen 1 */
            uint8_t g_sf_el_gx_canvas[sizeof(g_display_fb_background[0])] BSP_ALIGN_VARIABLE_V2(4) BSP_PLACE_IN_SECTION_V2(".bss");
            #else /* Inherit Frame Buffer Name from Graphics Screen 2 */
            uint8_t g_sf_el_gx_canvas[sizeof(g_display_fb_foreground[0])] BSP_ALIGN_VARIABLE_V2(4) BSP_PLACE_IN_SECTION_V2(".bss");
            #endif
            #endif

/** JPEG Work Buffer */
#if GX_USE_SYNERGY_JPEG
            #if (1137)
            uint8_t g_sf_el_gx_jpegbuffer[1137] BSP_ALIGN_VARIABLE_V2(64) BSP_PLACE_IN_SECTION_V2(".bss");
            #endif
            #endif

/** GUIX Port module control block instance */
static sf_el_gx_instance_ctrl_t g_sf_el_gx_ctrl;

/** GUIX Port module configuration */
static const sf_el_gx_cfg_t g_sf_el_gx_cfg =
{
/* Display Instance Configuration */
.p_display_instance = (display_instance_t*) &g_display,

  /* Display Driver Runtime Configuration */
#if (1 == 1) /* Inherit Frame Buffer Name from Graphics Screen 1 */
  .p_display_runtime_cfg = &g_display_runtime_cfg_bg,
#else /* Inherit Frame Buffer Name from Graphics Screen 2 */
                .p_display_runtime_cfg = &g_display_runtime_cfg_fg,
            #endif

  /* GUIX Canvas Configuration */
#if (false)
                .p_canvas        = g_sf_el_gx_canvas,
            #else
  .p_canvas = NULL,
#endif

  /* Display Driver Frame Buffer A Configuration */
#if (1 == 1) /* Inherit Frame Buffer Name from Graphics Screen 1 */
  .p_framebuffer_a = &g_display_fb_background[0], /* Always array[0] is used */
  .inherit_frame_layer = DISPLAY_FRAME_LAYER_1,
#else /* Inherit Frame Buffer Name from Graphics Screen 2 */
                .p_framebuffer_a = &g_display_fb_foreground[0], /* Always array[0] is used */
	         .inherit_frame_layer = DISPLAY_FRAME_LAYER_2,
            #endif

  /* Display Driver Frame Buffer B Configuration */
#if (1 == 1) /* Inherit Frame Buffer Name from Graphics Screen 1 */
#if (1 > 1) /* Multiple frame buffers are used for Graphics Screen 1 */
                .p_framebuffer_b = &g_display_fb_background[1], /* Always array[1] is used */
                #else /* Single Frame Buffer is used for Graphics Screen 1 */
  .p_framebuffer_b = NULL,
#endif
#else /* Inherit Frame Buffer Name from Graphics Screen 2 */
                #if (2 > 1) /* Multiple frame buffers are used for Graphics Screen 2 */
                .p_framebuffer_b = &g_display_fb_foreground[1], /* Always array[1] is used */
                #else /* Single Frame Buffer is used for Graphics Screen 2 */
                .p_framebuffer_b = NULL,
                #endif
            #endif

  /* User Callback Configuration */
  .p_callback = NULL,

  /* JPEG Work Buffer Configuration */
#if GX_USE_SYNERGY_JPEG
                .p_jpegbuffer    = g_sf_el_gx_jpegbuffer,
                .jpegbuffer_size = 1137,
                .p_sf_jpeg_decode_instance = (void *)&g_sf_jpeg_decode0,
            #else
  .p_jpegbuffer = NULL,
  .jpegbuffer_size = 0, .p_sf_jpeg_decode_instance = NULL,
#endif

  /* D/AVE 2D Buffer Cache */
  .dave2d_buffer_cache_enabled = true };

/** GUIX Port module instance */
sf_el_gx_instance_t g_sf_el_gx =
{ .p_api = &sf_el_gx_on_guix, .p_ctrl = &g_sf_el_gx_ctrl, .p_cfg = &g_sf_el_gx_cfg };
/* Instance structure to use this module. */
const fmi_instance_t g_fmi =
{ .p_api = &g_fmi_on_fmi };
const elc_instance_t g_elc =
{ .p_api = &g_elc_on_elc, .p_cfg = NULL };
const cgc_instance_t g_cgc =
{ .p_api = &g_cgc_on_cgc, .p_cfg = NULL };
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_cfg = NULL };
void g_common_init(void)
{

    /* Open Crypto Common Framework, only if 'Auto Initialization' property is enabled. */
#if (1)
    ssp_err_t ssp_err_g_sf_crypto0;
    ssp_err_g_sf_crypto0 = g_sf_crypto0.p_api->open (g_sf_crypto0.p_ctrl, g_sf_crypto0.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_crypto0)
    {
        BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
    }
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    nx_common_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    packet_pool_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    ip_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if FX_COMMON_INITIALIZE
    fx_common_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    ux_common_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    ux_host_init0 ();
#endif

#if (0)
            /** Call FileX Media Control Block initialization function. Can call at later time as well. */
            fx_media_init_function0();
#endif
}
