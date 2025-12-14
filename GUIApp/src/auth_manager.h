#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize auth backend (QSPI open). Returns 0 on success. */
int auth_init(void);

/* Returns 1 if a password record exists and is valid, 0 if not, <0 on error. */
int auth_is_provisioned(void);

/* Verify user credentials. Returns 1 if OK, 0 if wrong, <0 on error. */
int auth_verify_credentials(const char *username, const char *password);

/* Set / change admin password. Returns 0 on success, <0 on error. */
int auth_set_admin_password(const char *new_password);

/* Returns 1 if recovery is allowed (e.g., ALLOW_RESET.TXT on USB), 0 otherwise. */
int auth_recovery_allowed(void);

/* Diagnostics (for clear JSON errors) */
const char * auth_last_qspi_op(void);   /* "open", "read", "erase", "program", ... */
uint32_t     auth_last_qspi_err(void);  /* SSP error code from last failure/success */

#ifdef __cplusplus
}
#endif

#endif /* AUTH_MANAGER_H */
