

#ifndef BITOPS_H
#define BITOPS_H

#include <stdint.h>
#include <stddef.h>

#ifdef _MSC_VER
# define inline __inline
#endif

static inline uint32_t rotr32(uint32_t x, unsigned n)
{
  return (x >> n) | (x << (32 - n));
}


static inline uint32_t rotl32(uint32_t x, unsigned n)
{
  return (x << n) | (x >> (32 - n));
}


static inline uint64_t rotr64(uint64_t x, unsigned n)
{
  return (x >> n) | (x << (64 - n));
}

static inline uint64_t rotl64(uint64_t x, unsigned n)
{
  return (x << n) | (x >> (64 - n));
}

static inline uint32_t read32_be(const uint8_t buf[4])
{
  return (buf[0] << 24) |
         (buf[1] << 16) |
         (buf[2] << 8) |
         (buf[3]);
}


static inline uint32_t read32_le(const uint8_t buf[4])
{
  return (buf[3] << 24) |
         (buf[2] << 16) |
         (buf[1] << 8) |
         (buf[0]);
}
static inline uint64_t read64_be(const uint8_t buf[8])
{
  uint32_t hi = read32_be(buf),
           lo = read32_be(buf + 4);
  return ((uint64_t)hi) << 32 |
         lo;
}


static inline uint64_t read64_le(const uint8_t buf[8])
{
  uint32_t hi = read32_le(buf + 4),
           lo = read32_le(buf);
  return ((uint64_t)hi) << 32 |
         lo;
}

static inline void write32_be(uint32_t v, uint8_t buf[4])
{
  *buf++ = (v >> 24) & 0xff;
  *buf++ = (v >> 16) & 0xff;
  *buf++ = (v >> 8) & 0xff;
  *buf   = v & 0xff;
}


static inline void write32_le(uint32_t v, uint8_t buf[4])
{
  *buf++ = v & 0xff;
  *buf++ = (v >> 8) & 0xff;
  *buf++ = (v >> 16) & 0xff;
  *buf   = (v >> 24) & 0xff;
}

static inline void write64_be(uint64_t v, uint8_t buf[8])
{
  *buf++ = (v >> 56) & 0xff;
  *buf++ = (v >> 48) & 0xff;
  *buf++ = (v >> 40) & 0xff;
  *buf++ = (v >> 32) & 0xff;
  *buf++ = (v >> 24) & 0xff;
  *buf++ = (v >> 16) & 0xff;
  *buf++ = (v >> 8) & 0xff;
  *buf   = v & 0xff;
}
static inline void write64_le(uint64_t v, uint8_t buf[8])
{
  *buf++ = v & 0xff;
  *buf++ = (v >> 8) & 0xff;
  *buf++ = (v >> 16) & 0xff;
  *buf++ = (v >> 24) & 0xff;
  *buf++ = (v >> 32) & 0xff;
  *buf++ = (v >> 40) & 0xff;
  *buf++ = (v >> 48) & 0xff;
  *buf   = (v >> 56) & 0xff;
}

static inline void xor_b8(uint8_t *out, const uint8_t *in, uint8_t b8, size_t len)
{
  size_t i;
  for (i = 0; i < len; i++)
    out[i] = in[i] ^ b8;
}


static inline void xor_bb(uint8_t *out, const uint8_t *x, const uint8_t *y, size_t len)
{
  size_t i;
  for (i = 0; i < len; i++)
    out[i] = x[i] ^ y[i];
}


static inline void xor_words(uint32_t *out, const uint32_t *x, size_t nwords)
{
  size_t i;
  for (i = 0; i < nwords; i++)
    out[i] ^= x[i];
}


static inline uint32_t mask_u32(uint32_t x, uint32_t y)
{
  uint32_t diff = x ^ y;
  uint32_t diff_is_zero = ~diff & (diff - 1);
  return - (diff_is_zero >> 31);
}

static inline uint8_t mask_u8(uint32_t x, uint32_t y)
{
  uint8_t diff = x ^ y;
  uint8_t diff_is_zero = ~diff & (diff - 1);
  return - (diff_is_zero >> 7);
}


static inline uint32_t select_u32(uint32_t i, volatile const uint32_t *tab, uint32_t n)
{
  uint32_t r = 0;
  uint32_t ii;

  for (ii = 0; ii < n; ii++)
  {
    uint32_t mask = mask_u32(i, ii);
    r = (r & ~mask) | (tab[ii] & mask);
  }

  return r;
}

static inline uint8_t select_u8(uint32_t i, volatile const uint8_t *tab, uint32_t n)
{
  uint8_t r = 0;
  uint32_t ii;

  for (ii = 0; ii < n; ii++)
  {
    uint8_t mask = mask_u8(i, ii);
    r = (r & ~mask) | (tab[ii] & mask);
  }

  return r;
}


static inline void select_u8x4(uint8_t *a, uint8_t *b, uint8_t *c, uint8_t *d,
                               volatile const uint8_t *tab, uint32_t n)
{
  uint8_t ra = 0,
          rb = 0,
          rc = 0,
          rd = 0;
  uint8_t mask;
  uint32_t i;

  for (i = 0; i < n; i++)
  {
    uint8_t item = tab[i];

    mask = mask_u8(*a, i); ra = (ra & ~mask) | (item & mask);
    mask = mask_u8(*b, i); rb = (rb & ~mask) | (item & mask);
    mask = mask_u8(*c, i); rc = (rc & ~mask) | (item & mask);
    mask = mask_u8(*d, i); rd = (rd & ~mask) | (item & mask);
  }

  *a = ra;
  *b = rb;
  *c = rc;
  *d = rd;
}

static inline void select_xor128(uint32_t out[4],
                                 const uint32_t if0[4],
                                 const uint32_t if1[4],
                                 uint8_t bit)
{
  uint32_t mask1 = mask_u32(bit, 1);
  uint32_t mask0 = ~mask1;

  out[0] ^= (if0[0] & mask0) | (if1[0] & mask1);
  out[1] ^= (if0[1] & mask0) | (if1[1] & mask1);
  out[2] ^= (if0[2] & mask0) | (if1[2] & mask1);
  out[3] ^= (if0[3] & mask0) | (if1[3] & mask1);
}

static inline void incr_le(uint8_t *v, size_t len)
{
  size_t i = 0;
  while (1)
  {
    if (++v[i] != 0)
      return;
    i++;
    if (i == len)
      return;
  }
}

static inline void incr_be(uint8_t *v, size_t len)
{
  len--;
  while (1)
  {
    if (++v[len] != 0)
      return;
    if (len == 0)
      return;
    len--;
  }
}

#endif
