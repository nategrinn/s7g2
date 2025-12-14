#ifndef SHA256_MINI_H
#define SHA256_MINI_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t state[8];
    uint64_t bitlen;
    uint8_t  data[64];
    uint32_t datalen;
} sha256_ctx_t;

void sha256_init(sha256_ctx_t *ctx);
void sha256_update(sha256_ctx_t *ctx, const uint8_t *data, size_t len);
void sha256_final(sha256_ctx_t *ctx, uint8_t out[32]);

/* Convenience: one-shot hash. */
void sha256(const uint8_t *data, size_t len, uint8_t out[32]);

#ifdef __cplusplus
}
#endif

#endif
