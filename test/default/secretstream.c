
#define TEST_NAME "secretstream"
#include "cmptest.h"

int
main(void)
{
    crypto_secretstream_xchacha20poly1305_state *state;
    unsigned char *header;
    unsigned char *k;
    unsigned char *c1, *c2, *c3;
    unsigned char *m1, *m2, *m3;
    unsigned char *m1_, *m2_, *m3_;
    size_t         m1_len, m2_len, m3_len;
    int            ret;
    unsigned char  tag;

    state = (crypto_secretstream_xchacha20poly1305_state *)
        sodium_malloc(crypto_secretstream_xchacha20poly1305_statebytes());
    header = (unsigned char *)
        sodium_malloc(crypto_secretstream_xchacha20poly1305_INITBYTES);

    m1_len = randombytes_uniform(1000);
    m2_len = randombytes_uniform(1000);
    m3_len = randombytes_uniform(1000);

    c1 = (unsigned char *)
        sodium_malloc(m1_len + crypto_secretstream_xchacha20poly1305_ABYTES);
    c2 = (unsigned char *)
        sodium_malloc(m2_len + crypto_secretstream_xchacha20poly1305_ABYTES);
    c3 = (unsigned char *)
        sodium_malloc(m3_len + crypto_secretstream_xchacha20poly1305_ABYTES);

    m1  = (unsigned char *) sodium_malloc(m1_len);
    m2  = (unsigned char *) sodium_malloc(m2_len);
    m3  = (unsigned char *) sodium_malloc(m3_len);
    m1_ = (unsigned char *) sodium_malloc(m1_len);
    m2_ = (unsigned char *) sodium_malloc(m2_len);
    m3_ = (unsigned char *) sodium_malloc(m3_len);

    randombytes_buf(m1, m1_len);
    memcpy(m1_, m1, m1_len);
    randombytes_buf(m2, m2_len);
    memcpy(m2_, m2, m2_len);
    randombytes_buf(m3, m3_len);
    memcpy(m3_, m3, m3_len);

    k = (unsigned char *)
        sodium_malloc(crypto_secretstream_xchacha20poly1305_KEYBYTES);
    crypto_secretstream_xchacha20poly1305_keygen(k);

    /* push */

    ret = crypto_secretstream_xchacha20poly1305_init_push(state, header, k);
    assert(ret == 0);

    ret = crypto_secretstream_xchacha20poly1305_push
        (state, c1, NULL, m1, m1_len, NULL, 0, 0);
    assert(ret == 0);

    ret = crypto_secretstream_xchacha20poly1305_push
        (state, c2, NULL, m2, m2_len, NULL, 0, 0);
    assert(ret == 0);

    ret = crypto_secretstream_xchacha20poly1305_push
        (state, c3, NULL, m3, m3_len, NULL, 0,
         crypto_secretstream_xchacha20poly1305_TAG_FINAL);
    assert(ret == 0);

    /* pull */

    ret = crypto_secretstream_xchacha20poly1305_init_pull(state, header, k);
    assert(ret == 0);

    ret = crypto_secretstream_xchacha20poly1305_pull
        (state, m1, NULL, &tag,
         c1, m1_len + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0);
    assert(ret == 0);
    assert(tag == 0);
    assert(memcmp(m1, m1_, m1_len) == 0);

    ret = crypto_secretstream_xchacha20poly1305_pull
        (state, m2, NULL, &tag,
         c2, m2_len + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0);
    assert(ret == 0);
    assert(tag == 0);
    assert(memcmp(m2, m2_, m2_len) == 0);

    ret = crypto_secretstream_xchacha20poly1305_pull
        (state, m3, NULL, &tag,
         c3, m3_len + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0);
    assert(ret == 0);
    assert(tag == crypto_secretstream_xchacha20poly1305_TAG_FINAL);
    assert(memcmp(m3, m3_, m3_len) == 0);

    /* previous with FINAL tag */

    ret = crypto_secretstream_xchacha20poly1305_pull
        (state, m3, NULL, &tag,
         c3, m3_len + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0);
    assert(ret == -1);

    /* previous without a tag */

    ret = crypto_secretstream_xchacha20poly1305_pull
        (state, m2, NULL, &tag,
         c2, m2_len + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0);
    assert(ret == -1);

    /* without explicit rekeying */

    ret = crypto_secretstream_xchacha20poly1305_init_push(state, header, k);
    assert(ret == 0);
    ret = crypto_secretstream_xchacha20poly1305_push
        (state, c1, NULL, m1, m1_len, NULL, 0, 0);
    assert(ret == 0);
    ret = crypto_secretstream_xchacha20poly1305_push
        (state, c2, NULL, m2, m2_len, NULL, 0, 0);
    assert(ret == 0);

    ret = crypto_secretstream_xchacha20poly1305_init_pull(state, header, k);
    assert(ret == 0);
    ret = crypto_secretstream_xchacha20poly1305_pull
        (state, m1, NULL, &tag,
         c1, m1_len + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0);
    assert(ret == 0);
    ret = crypto_secretstream_xchacha20poly1305_pull
        (state, m2, NULL, &tag,
         c2, m2_len + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0);
    assert(ret == 0);

    /* with explicit rekeying */

    ret = crypto_secretstream_xchacha20poly1305_init_push(state, header, k);
    assert(ret == 0);
    ret = crypto_secretstream_xchacha20poly1305_push
        (state, c1, NULL, m1, m1_len, NULL, 0, 0);
    assert(ret == 0);

    crypto_secretstream_xchacha20poly1305_rekey(state);

    ret = crypto_secretstream_xchacha20poly1305_push
        (state, c2, NULL, m2, m2_len, NULL, 0, 0);
    assert(ret == 0);

    ret = crypto_secretstream_xchacha20poly1305_init_pull(state, header, k);
    assert(ret == 0);
    ret = crypto_secretstream_xchacha20poly1305_pull
        (state, m1, NULL, &tag,
         c1, m1_len + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0);
    assert(ret == 0);

    ret = crypto_secretstream_xchacha20poly1305_pull
        (state, m2, NULL, &tag,
         c2, m2_len + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0);
    assert(ret == -1);

    crypto_secretstream_xchacha20poly1305_rekey(state);

    ret = crypto_secretstream_xchacha20poly1305_pull
        (state, m2, NULL, &tag,
         c2, m2_len + crypto_secretstream_xchacha20poly1305_ABYTES, NULL, 0);
    assert(ret == 0);

    sodium_free(m3_);
    sodium_free(m2_);
    sodium_free(m1_);
    sodium_free(m3);
    sodium_free(m2);
    sodium_free(m1);
    sodium_free(c3);
    sodium_free(c2);
    sodium_free(c1);
    sodium_free(k);
    sodium_free(header);
    sodium_free(state);
    
    printf("OK\n");

    return 0;
}
