#ifndef CHASH_H
#define CHASH_H

#include <stddef.h>
#include <stdint.h>

typedef void (*cf_chash_init)(void *ctx);


typedef void (*cf_chash_update)(void *ctx, const void *data, size_t count);


typedef void (*cf_chash_digest)(const void *ctx, uint8_t *hash);

typedef struct
{
  size_t hashsz;
  size_t blocksz;

  cf_chash_init init;
  cf_chash_update update;
  cf_chash_digest digest;
} cf_chash;


#define CF_CHASH_MAXCTX 360


#define CF_CHASH_MAXBLK 128


#define CF_MAXHASH 64


typedef union
{
  uint8_t ctx[CF_CHASH_MAXCTX];
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;
} cf_chash_ctx;


void cf_hash(const cf_chash *h, const void *m, size_t nm, uint8_t *out);

#endif
