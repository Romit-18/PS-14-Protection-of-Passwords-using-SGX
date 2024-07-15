#ifndef BLOCKWISE_H
#define BLOCKWISE_H

#include <stdint.h>
#include <stddef.h>

/* Processing function for cf_blockwise_accumulate. */
typedef void (*cf_blockwise_in_fn)(void *ctx, const uint8_t *data);

/* Processing function for cf_blockwise_xor. */
typedef void (*cf_blockwise_out_fn)(void *ctx, uint8_t *data);


void cf_blockwise_accumulate(uint8_t *partial, size_t *npartial,
                             size_t nblock,
                             const void *input, size_t nbytes,
                             cf_blockwise_in_fn process, 
                             void *ctx);


void cf_blockwise_accumulate_final(uint8_t *partial, size_t *npartial,
                                   size_t nblock,
                                   const void *input, size_t nbytes,
                                   cf_blockwise_in_fn process, 
                                   cf_blockwise_in_fn process_final,
                                   void *ctx);


void cf_blockwise_xor(uint8_t *partial, size_t *npartial,
                      size_t nblock,
                      const void *input, void *output, size_t nbytes,
                      cf_blockwise_out_fn newblock,
                      void *ctx);

void cf_blockwise_acc_byte(uint8_t *partial, size_t *npartial,
                           size_t nblock,
                           uint8_t byte, size_t nbytes,
                           cf_blockwise_in_fn process,
                           void *ctx);


void cf_blockwise_acc_pad(uint8_t *partial, size_t *npartial,
                          size_t nblock,
                          uint8_t fbyte, uint8_t mbyte, uint8_t lbyte,
                          size_t nbytes,
                          cf_blockwise_in_fn process,
                          void *ctx);

#endif
