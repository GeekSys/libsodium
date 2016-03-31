#ifndef crypto_secretbox_xsalsa20crc32_H
#define crypto_secretbox_xsalsa20crc32_H

#include <stddef.h>
#include "export.h"

#ifdef __cplusplus
# if __GNUC__
#  pragma GCC diagnostic ignored "-Wlong-long"
# endif
extern "C" {
#endif

#define crypto_secretbox_xsalsa20crc32_KEYBYTES 32U
SODIUM_EXPORT
size_t crypto_secretbox_xsalsa20crc32_keybytes(void);

#define crypto_secretbox_xsalsa20crc32_NONCEBYTES 24U
SODIUM_EXPORT
size_t crypto_secretbox_xsalsa20crc32_noncebytes(void);

#define crypto_secretbox_xsalsa20crc32_MACBYTES 4U
SODIUM_EXPORT
size_t crypto_secretbox_xsalsa20crc32_macbytes(void);

#define crypto_secretbox_xsalsa20crc32_BOXZEROBYTES 0U
SODIUM_EXPORT
size_t crypto_secretbox_xsalsa20crc32_boxzerobytes(void);

#define crypto_secretbox_xsalsa20crc32_ZEROBYTES \
    (crypto_secretbox_xsalsa20crc32_BOXZEROBYTES + \
     crypto_secretbox_xsalsa20crc32_MACBYTES)
SODIUM_EXPORT
size_t crypto_secretbox_xsalsa20crc32_zerobytes(void);

SODIUM_EXPORT
int crypto_secretbox_xsalsa20crc32(unsigned char *c,
                                   const unsigned char *m,
                                   unsigned long long mlen,
                                   const unsigned char *n,
                                   const unsigned char *k);

SODIUM_EXPORT
int crypto_secretbox_xsalsa20crc32_open(unsigned char *m,
                                        const unsigned char *c,
                                        unsigned long long clen,
                                        const unsigned char *n,
                                        const unsigned char *k);

#ifdef __cplusplus
}
#endif

#endif
