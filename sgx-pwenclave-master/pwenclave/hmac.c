
#include "hmac.h"
#include "chash.h"
#include "bitops.h"
#include "handy.h"
#include "tassert.h"

#include <string.h>

void cf_hmac_init(cf_hmac_ctx *ctx,
                  const cf_chash *hash,
                  const uint8_t *key, size_t nkey)
{
  uint8_t k[CF_CHASH_MAXBLK];
  uint8_t blk[CF_CHASH_MAXBLK];

  assert(ctx);
  assert(hash);

  mem_clean(ctx, sizeof *ctx);
  ctx->hash = hash;

  /* Prepare key: */

  /* Shorten long keys. */
  if (nkey > hash->blocksz)
  {
    /* Standard doesn't cover case where blocksz < hashsz.
     * FIPS186-1 seems to want to append a negative number of zero bytes.
     * In any case, we only have a k buffer of CF_CHASH_MAXBLK! */
    assert(hash->hashsz <= hash->blocksz);

    cf_hash(hash, key, nkey, k);
    key = k;
    nkey = hash->hashsz;
  }

  /* Right zero-pad short keys. */
  if (k != key)
    memcpy(k, key, nkey);
  if (hash->blocksz > nkey)
    memset(k + nkey, 0, hash->blocksz - nkey);

  /* Start inner hash computation */
  xor_b8(blk, k, 0x36, hash->blocksz);
  hash->init(&ctx->inner);
  hash->update(&ctx->inner, blk, hash->blocksz);

  /* And outer. */
  xor_b8(blk, k, 0x5c, hash->blocksz);
  hash->init(&ctx->outer);
  hash->update(&ctx->outer, blk, hash->blocksz);

  mem_clean(blk, sizeof blk);
  mem_clean(k, sizeof k);
}

void cf_hmac_update(cf_hmac_ctx *ctx, const void *data, size_t ndata)
{
  assert(ctx && ctx->hash);

  ctx->hash->update(&ctx->inner, data, ndata);
}

void cf_hmac_finish(cf_hmac_ctx *ctx, uint8_t *out)
{
  uint8_t innerh[CF_MAXHASH];

  assert(ctx && ctx->hash);
  assert(out);

  ctx->hash->digest(&ctx->inner, innerh);

  ctx->hash->update(&ctx->outer, innerh, ctx->hash->hashsz);
  ctx->hash->digest(&ctx->outer, out);

  mem_clean(ctx, sizeof *ctx);
}

void cf_hmac(const uint8_t *key, size_t nkey,
             const uint8_t *msg, size_t nmsg,
             uint8_t *out,
             const cf_chash *hash)
{
  cf_hmac_ctx ctx;
  
  assert(out);
  assert(hash);

  cf_hmac_init(&ctx, hash, key, nkey);
  cf_hmac_update(&ctx, msg, nmsg);
  cf_hmac_finish(&ctx, out);
}

