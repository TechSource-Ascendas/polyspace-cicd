/**
 *       Copyright 2015-2025 The MathWorks, Inc.
 */

/**
 * Polyspace Bug finder example.
 * This example shows defects caused by cryptography errors.
 *
 * This example will show you how selected defects will be handled by the
 * Polyspace Bug Finder.
 * Each defect is depicted in a specific function, and a sibling function
 * showing the fixed code is then shown.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
#define __i386__
#define _GNU_SOURCE
#define _XOPEN_SOURCE 500
#include <crypt.h>
#endif /* __GNUC__ */

#include "globaldefs.h"
#include "lib_crypto_checkers.h"


/**************************
 * SYMMETRIC CRYPTOGRAPHY *
 **************************/


/*============================================================================
 * PREDICTABLE KEY
 *==========================================================================*/
static int demo_predictablekeyaux(unsigned char* key) {
    return RAND_pseudo_bytes(key, SIZE16); /* RAND_pseudo_bytes generates predictable output */
}

int bug_cryptocipherpredictablekey(EVP_CIPHER_CTX* ctx, unsigned char* iv) {
    unsigned char* key = (unsigned char*)malloc(SIZE16 * sizeof(unsigned char));
    demo_predictablekeyaux(key);
    return EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv,
                             1); /* Defect: key is predictable and thus vulnerable */
}

int corrected_cryptocipherpredictablekey(EVP_CIPHER_CTX* ctx, unsigned char* iv) {
    unsigned char key[SIZE16];
    RAND_bytes(key, SIZE16);
    return EVP_CipherInit_ex(
        ctx, EVP_aes_128_cbc(), NULL, key, iv,
        1); /* Fix: key generated with RAND_bytes which is a secure random generator */
}


/*============================================================================
 * PREDICTABLE IV
 *==========================================================================*/
static int demo_predictableivaux(unsigned char* iv) {
    return RAND_pseudo_bytes(iv, SIZE16); /* RAND_pseudo_bytes generates predictable output  */
}

int bug_cryptocipherpredictableiv(EVP_CIPHER_CTX* ctx, unsigned char* key) {
    unsigned char* iv = (unsigned char*)malloc(SIZE16 * sizeof(unsigned char));
    demo_predictableivaux(iv);
    return EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv,
                             1); /* Defect: IV is predictable and thus vulnerable */
}

int corrected_cryptocipherpredictableiv(EVP_CIPHER_CTX* ctx, unsigned char* key) {
    unsigned char iv[SIZE16];
    RAND_bytes(iv,
               SIZE16); /*  Fix: iv generated with RAND_bytes which is a secure random generator */
    return EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv, 1);
}


/*============================================================================
 * CONSTANT KEY
 *==========================================================================*/
int bug_cryptocipherconstantkey(EVP_CIPHER_CTX* ctx, unsigned char* iv) {
    unsigned char key[SIZE16] = {'1', '2', '3', '4', '5', '6', 'b', '8',
                                 '9', '1', '2', '3', '4', '5', '6', '7'};
    return EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv,
                             1); /* Defect: key is hard-coded and thus vulnerable */
}

int corrected_cryptocipherconstantkey(EVP_CIPHER_CTX* ctx, unsigned char* iv) {
    unsigned char key[SIZE16];
    RAND_bytes(key, SIZE16);
    return EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv,
                             1); /* Fix: key generated with a strong random generator */
}


/*============================================================================
 * CONSTANT IV
 *==========================================================================*/
int bug_cryptocipherconstantiv(EVP_CIPHER_CTX* ctx, unsigned char* key) {
    unsigned char iv[SIZE16] = {'1', '2', '3', '4', '5', '6', 'b', '8',
                                '9', '1', '2', '3', '4', '5', '6', '7'};
    return EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv,
                             1); /* Defect: iv is constant, thus encrypted data
                                    is weak against dictionary attacks */
}

int corrected_cryptocipherconstantiv(EVP_CIPHER_CTX* ctx, unsigned char* key) {
    unsigned char iv[SIZE16];
    RAND_bytes(iv, SIZE16); /* Fix: IV generated with a strong random generator */
    return EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv, 1);
}


/*============================================================================
 * NO KEY
 *==========================================================================*/
unsigned char* out_buf1;
int out_len1;
int bug_cryptociphernokey(EVP_CIPHER_CTX* ctx, unsigned char* iv, unsigned char* src, int len) {
    if (iv == NULL) {
        fatal_error();
    }

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, NULL, iv);
    return EVP_EncryptUpdate(ctx, out_buf1, &out_len1, src,
                             len); /* Defect: no key has been set, this step will fail*/
}

int corrected_cryptociphernokey(EVP_CIPHER_CTX* ctx,
                                unsigned char* iv,
                                unsigned char* src,
                                int len) {
    if (iv == NULL) {
        fatal_error();
    }

    unsigned char key[SIZE16];
    RAND_bytes(key, SIZE16);
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    return EVP_EncryptUpdate(ctx, out_buf1, &out_len1, src,
                             len); /* Fix: a non null key has been provided
                                     in an initialization step */
}


/*============================================================================
 * NO IV
 *==========================================================================*/
unsigned char* out_buf2;
int out_len2;
int bug_cryptociphernoiv(EVP_CIPHER_CTX* ctx, unsigned char* key, unsigned char* src, int len) {
    if (key == NULL) {
        fatal_error();
    }

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL); /* Null iv (last argument) */
    return EVP_EncryptUpdate(ctx, out_buf2, &out_len2, src,
                             len); /* Defect: no iv has been set. Data can be encrypted
                                     but will be weak against dictionary attacks */
}

int corrected_cryptociphernoiv(EVP_CIPHER_CTX* ctx,
                               unsigned char* key,
                               unsigned char* src,
                               int len) {
    if (key == NULL) {
        fatal_error();
    }

    unsigned char iv[SIZE16];
    RAND_bytes(iv, SIZE16);
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    return EVP_EncryptUpdate(ctx, out_buf2, &out_len2, src,
                             len); /* Fix: a non null iv has been provided
                                      in an initialization step */
}


/*============================================================================
 * NO ALGO
 *==========================================================================*/
unsigned char g_key1[SIZE16];
unsigned char g_iv1[SIZE16];
void bug_cryptociphernoalgorithm(void) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new(); /* new context, needing a cipher to be set */
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, NULL, NULL, g_key1,
                       g_iv1); /* Defect: no cipher set, so this initialization is useless:
                                  the update step cannot be performed and the context
                                  may be overwritten by an initialization with a non null cipher */
}
void corrected_cryptociphernoalgorithm(unsigned char* src,
                                       int len,
                                       unsigned char* out_buf,
                                       int out_len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, g_key1, g_iv1);
    EVP_EncryptUpdate(ctx, out_buf, &out_len, src, len);
}


/*============================================================================
 * NO DATA
 *==========================================================================*/
unsigned char* out_buf3;
int out_len3;
unsigned char g_key2[SIZE16];
unsigned char g_iv2[SIZE16];
void bug_cryptociphernodata(void) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, g_key2, g_iv2);
    EVP_EncryptFinal_ex(
        ctx, out_buf3,
        &out_len3); /* Defect: at least one update step was expected to provide data */
}
void corrected_cryptociphernodata(unsigned char* src, int len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, g_key2, g_iv2);
    EVP_EncryptUpdate(ctx, out_buf3, &out_len3, src, len); /* Fix: update step providing data src */
    EVP_EncryptFinal_ex(ctx, out_buf3, &out_len3);
}


/*============================================================================
 * NO FINAL
 *==========================================================================*/
unsigned char* out_buf4;
int out_len4;
unsigned char g_key3[SIZE16];
unsigned char g_iv3[SIZE16];
void bug_cryptociphernofinal(unsigned char* src, int len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, g_key3, g_iv3);
    EVP_EncryptUpdate(ctx, out_buf4, &out_len4, src, len); /* update step */
    EVP_CIPHER_CTX_cleanup(ctx); /* Defect: final step should occur after update step(s) */
}
void corrected_cryptociphernofinal(unsigned char* src, int len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, g_key3, g_iv3);
    EVP_EncryptUpdate(ctx, out_buf4, &out_len4, src, len);
    EVP_EncryptFinal_ex(ctx, out_buf4, &out_len4); /* final step */
    EVP_CIPHER_CTX_cleanup(ctx); /* Fix: init, update and final steps has been called
                                    output is complete and ready to be used, ctx can be cleaned  */
}


/*============================================================================
 * BAD FUNCTION
 *==========================================================================*/
unsigned char* out_buf5;
int out_len5;
unsigned char g_key4[SIZE16];
unsigned char g_iv4[SIZE16];
void bug_cryptocipherbadfunction(unsigned char* src, int len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, g_key4, g_iv4);
    EVP_EncryptUpdate(ctx, out_buf5, &out_len5, src,
                      len); /* Defect: an encryption step is performed
                               while the context has been initialized for decryption */
}
void corrected_cryptocipherbadfunction(unsigned char* src, int len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, g_key4, g_iv4);
    EVP_EncryptUpdate(ctx, out_buf5, &out_len5, src,
                      len); /* Fix: init and update step are both designed for encryption */
}


/*============================================================================
 * WEAK CIPHER
 *==========================================================================*/
void bug_cryptocipherweakcipher(unsigned char* lkey, unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    const EVP_CIPHER* ciph = EVP_des_cbc(); /* DES is vulnerable (and slow) */
    EVP_EncryptInit_ex(ctx, ciph, NULL, lkey,
                       iv); /* Defect: use of DES as algorithm for encryption */
}
void corrected_cryptocipherweakcipher(unsigned char* lkey, unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    const EVP_CIPHER* ciph = EVP_aes_128_cbc();    /* AES is secure (and fast) */
    EVP_EncryptInit_ex(ctx, ciph, NULL, lkey, iv); /* Fix: use of AES as algorithm */
}


/*============================================================================
 * WEAK MODE
 *==========================================================================*/
void bug_cryptocipherweakmode(unsigned char* lkey, unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    const EVP_CIPHER* ciph = EVP_aes_128_ecb(); /* ECB mode is a weak mode  */
    EVP_EncryptInit_ex(ctx, ciph, NULL, lkey,
                       iv); /* Defect: use of ECB as algorithm mode for encryption */
}
void corrected_cryptocipherweakmode(unsigned char* lkey, unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    const EVP_CIPHER* ciph = EVP_aes_128_cbc();    /* CBC mode is not a weak mode */
    EVP_EncryptInit_ex(ctx, ciph, NULL, lkey, iv); /* Fix: use of CBC as algorithm mode  */
}


/**********************************************************
 * ASYMMETRIC CRYPTOGRAPHIC (AKA PUBLIC KEY CRYPTOGRAPHY) *
 **********************************************************/


/*============================================================================
 * PKEY NO PUBLIC KEY
 *==========================================================================*/
unsigned char* out_buf6;
size_t out_len6;
int bug_cryptopkeynopublickey(unsigned char* src, size_t len) {
    int ret;
    EVP_PKEY_CTX* ctx =
        EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL); /* context created with only RSA parameters */
    if (ctx == NULL) {
        fatal_error();
    }
    ret = EVP_PKEY_encrypt_init(ctx);

    if (ret <= 0) {
        fatal_error();
    }
    return EVP_PKEY_encrypt(ctx, out_buf6, &out_len6, src, len); /* Defect: no public key has been
                                                                  set, this encryption will fail*/
}

int corrected_cryptopkeynopublickey(unsigned char* src, size_t len, EVP_PKEY* pkey) {
    int ret;
    if (pkey == NULL) {
        fatal_error(); /*  the given key object is provided */
    }

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pkey, NULL); /* context created with the given key object
                                                       that contains a public key */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_encrypt_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_encrypt(ctx, out_buf6, &out_len6, src,
                            len); /* Fix: a public key has been provided
                                   in the context for this encryption */
}


/*============================================================================
 * PKEY NO PRIVATE KEY
 *==========================================================================*/
unsigned char* out_buf7;
size_t out_len7;
int bug_cryptopkeynoprivatekey(unsigned char* src, size_t len) {
    int ret;
    EVP_PKEY_CTX* ctx =
        EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL); /* context created with only RSA parameters */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_decrypt_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_decrypt(ctx, out_buf7, &out_len7, src, len); /* Defect: no private key has been
                                                                  set, this encryption will fail*/
}

int corrected_cryptopkeynoprivatekey(unsigned char* src, size_t len, EVP_PKEY* pkey) {
    int ret;
    if (pkey == NULL) {
        fatal_error(); /* the given key object is provided */
    }

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pkey, NULL); /* context created with the given key object
                                                       that contains a private key */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_decrypt_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_decrypt(ctx, out_buf7, &out_len7, src,
                            len); /* Fix: a private key has been provided
                                   in the context for this encryption */
}


/*============================================================================
 * PKEY NO PEER
 *==========================================================================*/
unsigned char* out_buf8;
size_t out_len8;
int bug_cryptopkeynopeer(EVP_PKEY* pkey) {
    int ret;
    if (pkey == NULL) {
        fatal_error(); /* the given key object is provided */
    }

    EVP_PKEY_CTX* ctx =
        EVP_PKEY_CTX_new(pkey, NULL); /* new context, no peer key is set at this step */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_derive_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_derive(ctx, out_buf8, &out_len8); /* Defect: no peer key has been set,
                                                       this key derivation will fail*/
}

int corrected_cryptopkeynopeer(EVP_PKEY* pkey, EVP_PKEY* peerkey) {
    int ret;
    if (pkey == NULL) {
        fatal_error(); /*  the given key object is provided */
    }

    if (peerkey == NULL) {
        fatal_error(); /* the given peer key is provided*/
    }

    EVP_PKEY_CTX* ctx =
        EVP_PKEY_CTX_new(pkey, NULL); /* new context, no peer key is set at this step */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_derive_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_derive_set_peer(ctx,
                                   peerkey); /* the given peer key is set in the context object */
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_derive(ctx, out_buf8, &out_len8); /* Fix: a peer key has been provided
                                                       in the context for this key derivation */
}


/*============================================================================
 * PKEY NO PARAMS
 *==========================================================================*/
int bug_cryptopkeynoparams(EVP_PKEY* pkey) {
    int ret;
    EVP_PKEY* params = EVP_PKEY_new(); /* the created parameter object is an empty structure*/
    if (params == NULL) {
        fatal_error();
    }

    EVP_PKEY_CTX* ctx =
        EVP_PKEY_CTX_new(params, NULL); /* no parameter is set at the creation of
                                         the context object (as the parameter object is empty)*/
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_keygen_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_keygen(ctx, &pkey); /* Defect: no parameter has been set (used algorithm, key
                                         length, etc.), this key generation will fail */
}

int corrected_cryptopkeynoparams(EVP_PKEY* pkey) {
    int ret;
    EVP_PKEY_CTX* ctx =
        EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL); /* context created with RSA parameters */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_keygen_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(
        ctx, 2048); /* the given RSA key length is set in the context*/
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_keygen(
        ctx, &pkey); /* Fix: RSA parameters is set in the context for this key generation */
}


/*============================================================================
 * PKEY NO DATA
 *==========================================================================*/
int bug_cryptopkeynodata(EVP_PKEY_CTX* ctx) {
    int ret;
    if (ctx == NULL) {
        fatal_error(); /* The given context object is provided */
    }
    unsigned char* sig = (unsigned char*)"0123456789";
    unsigned char* md = (unsigned char*)"0123456789";

    ret = EVP_PKEY_verify_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256());
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_verify(ctx, sig, 0, md,
                           0); /* Defect: no length has been set for the two signatures
                                to check, this signature verification will fail*/
}

int corrected_cryptopkeynodata(EVP_PKEY_CTX* ctx) {
    int ret;
    if (ctx == NULL) {
        fatal_error(); /* The given context object is provided */
    }
    unsigned char* sig = (unsigned char*)"0123456789";
    unsigned char* md = (unsigned char*)"0123456789";

    ret = EVP_PKEY_verify_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256());
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_verify(ctx, sig, 10, md, 10); /* Fix: the length is given for the two signatures
                                                   to check for this signature verification */
}


/*============================================================================
 * PKEY WEAK PARAMS
 *==========================================================================*/
int bug_cryptopkeyweakparams(EVP_PKEY* pkey) {
    int ret;
    EVP_PKEY_CTX* ctx =
        EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL); /* context created with RSA parameters */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_keygen_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx,
                                           512); /* the given RSA key length is set in the context*/
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_keygen(ctx, &pkey); /* Defect: a too short value has been set as key length for
                                         RSA, the use of the generated key is unsafe*/
}

int corrected_cryptopkeyweakparams(EVP_PKEY* pkey) {
    int ret;
    EVP_PKEY_CTX* ctx =
        EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL); /* context created with RSA parameters */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_keygen_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(
        ctx, 2048); /* the given RSA key length is set in the context*/
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_keygen(
        ctx, &pkey); /* Fix: a strong enough value has been set as key length for RSA */
}


/*============================================================================
 * PKEY INCORRECT KEY
 *==========================================================================*/
unsigned char* out_buf9;
size_t out_len9;
int bug_cryptopkeyincorrectkey(unsigned char* src, size_t len, DSA* dsa) {
    int ret;
    EVP_PKEY_CTX* ctx;
    EVP_PKEY* pkey = NULL;

    pkey = EVP_PKEY_new();
    if (pkey == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_set1_DSA(pkey, dsa);
    if (ret <= 0) {
        fatal_error();
    }

    ctx = EVP_PKEY_CTX_new(pkey, NULL); /* DSA key is set in the context */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_encrypt_init(ctx); /* an encryption operation is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_encrypt(ctx, out_buf9, &out_len9, src, len); /* Defect: DSA key is only
                                                                  for signature operation */
}

int corrected_cryptopkeyincorrectkey(unsigned char* src, size_t len, RSA* rsa) {
    int ret;
    EVP_PKEY_CTX* ctx;
    EVP_PKEY* pkey = NULL;

    pkey = EVP_PKEY_new();
    if (pkey == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_set1_RSA(pkey, rsa);
    if (ret <= 0) {
        fatal_error();
    }

    ctx = EVP_PKEY_CTX_new(pkey, NULL); /* RSA key is set in the context */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_encrypt_init(ctx); /* an encryption operation is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_encrypt(ctx, out_buf9, &out_len9, src, len); /* Fix: a RSA key is used for this
                                                                  encryption, which is compatible */
}


/*============================================================================
 * PKEY INCORRECT INIT
 *==========================================================================*/
unsigned char* out_buf10;
size_t out_len10;
int bug_cryptopkeyincorrectinit(unsigned char* src, size_t len, EVP_PKEY_CTX* ctx) {
    int ret;
    if (ctx == NULL) {
        fatal_error(); /* The given context object is provided */
    }

    ret = EVP_PKEY_decrypt_init(ctx); /* A decryption operation is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_encrypt(ctx, out_buf10, &out_len10, src,
                            len); /* Defect: a decryption operation has been
                                   initialized, this encryption will fail */
}

int corrected_cryptopkeyincorrectinit(unsigned char* src, size_t len, EVP_PKEY_CTX* ctx) {
    int ret;
    if (ctx == NULL) {
        fatal_error(); /* The given context object is provided */
    }

    ret = EVP_PKEY_encrypt_init(ctx); /* An encryption operation is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_encrypt(ctx, out_buf10, &out_len10, src,
                            len); /* Fix: an encryption operation has been
                                   initialized in the context*/
}


/*============================================================================
 * RSA LOW EXPONENT
 *==========================================================================*/
int bug_cryptorsalowexponent(EVP_PKEY* pkey) {
    int ret;
    BIGNUM* pubexp;
    EVP_PKEY_CTX* ctx;

    pubexp = BN_new(); /* creation of the public key exponent object*/
    if (pubexp == NULL) {
        fatal_error();
    }

    ret = BN_set_word(pubexp, 3); /* the given value is set in the public exponent object */
    if (ret <= 0) {
        fatal_error();
    }

    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL); /* context created with RSA parameters */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_keygen_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048); /* context created with RSA parameters */
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_keygen_pubexp(
        ctx, pubexp); /* the given public key exponent is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_keygen(ctx, &pkey); /* Defect: a too short value has been set as public exponent
                                         for RSA, the use of the generated key is unsafe*/
}

int corrected_cryptorsalowexponent(EVP_PKEY* pkey) {
    int ret;
    BIGNUM* pubexp;
    EVP_PKEY_CTX* ctx;

    pubexp = BN_new(); /* creation of the public key exponent object*/
    if (pubexp == NULL) {
        fatal_error();
    }

    ret = BN_set_word(pubexp, 65537); /* the given value is set in the public exponent object */
    if (ret <= 0) {
        fatal_error();
    }

    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL); /* context created with RSA parameters */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_keygen_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048); /* context created with RSA parameters */
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_keygen_pubexp(
        ctx, pubexp); /* the given public key exponent is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_keygen(
        ctx, &pkey); /* Fix: a long enough value has been set as public exponent for RSA */
}


/*============================================================================
 * RSA NO PADDING
 *==========================================================================*/
unsigned char* out_buf11;
size_t out_len11;
int bug_cryptorsanopadding(unsigned char* src, size_t len) {
    int ret;
    EVP_PKEY_CTX* ctx;
    EVP_PKEY* pkey;

    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL); /* context object for RSA key generation */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_keygen_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_keygen(ctx, &pkey); /* key generation for RSA */
    if (ret <= 0) {
        fatal_error();
    }

    EVP_PKEY_CTX_free(ctx);
    ctx = EVP_PKEY_CTX_new(pkey, NULL); /* context object for RSA encryption*/
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_encrypt_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_encrypt(ctx, out_buf11, &out_len11, src,
                            len); /* Defect: no padding is set,
                                 this encryption is not secure enough */
}

int corrected_cryptorsanopadding(unsigned char* src, size_t len) {
    int ret;
    EVP_PKEY_CTX* ctx;
    EVP_PKEY* pkey;

    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL); /* context object for RSA key generation */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_keygen_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_keygen(ctx, &pkey); /* key generation for RSA */
    if (ret <= 0) {
        fatal_error();
    }

    EVP_PKEY_CTX_free(ctx);
    ctx = EVP_PKEY_CTX_new(pkey, NULL); /* context object for RSA encryption*/
    if (ctx == NULL) {
        fatal_error();
    }

    ret = EVP_PKEY_encrypt_init(ctx);
    if (ret <= 0) {
        fatal_error();
    }

    ret = EVP_PKEY_CTX_set_rsa_padding(
        ctx, RSA_PKCS1_OAEP_PADDING); /* the given padding is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    return EVP_PKEY_encrypt(ctx, out_buf11, &out_len11, src,
                            len); /* Fix: OAEP is set for this encryption */
}


/*============================================================================
 * RSA WEAK PADDING
 *==========================================================================*/
unsigned char* out_buf12;
size_t out_len12;
int bug_cryptorsaweakpadding(unsigned char* src, size_t len, RSA* rsa) {
    if (rsa == NULL) {
        fatal_error(); /* The given context is provided */
    }

    return RSA_public_encrypt(len, src, out_buf12, rsa,
                              RSA_PKCS1_PADDING); /* Defect: the used padding is broken,
                                                   this encryption is not secure enough */
}

int corrected_cryptorsaweakpadding(unsigned char* src, size_t len, RSA* rsa) {
    if (rsa == NULL) {
        fatal_error(); /* The given context is provided */
    }

    return RSA_public_encrypt(len, src, out_buf12, rsa,
                              RSA_PKCS1_OAEP_PADDING); /* Fix: OAEP is a secure padding,
                                                        this encryption is secure */
}


/*============================================================================
 * RSA BAD PADDING
 *==========================================================================*/
unsigned char* out_buf13;
unsigned char* msg_pad;
int bug_cryptorsabadpadding(unsigned char* src, size_t len, RSA* rsa) {
    if (rsa == NULL) {
        fatal_error(); /* The given context is provided */
    }

    return RSA_private_encrypt(len, src, out_buf13, rsa,
                               RSA_PKCS1_OAEP_PADDING); /* Defect: OAEP padding is only
                                                         used for encryption,
                                                         not for signature operations */
}

int corrected_cryptorsabadpadding(unsigned char* src, size_t len, RSA* rsa) {
    int ret;
    if (rsa == NULL) {
        fatal_error(); /* The given context is provided */
    }

    ret = RSA_padding_add_PKCS1_PSS(rsa, msg_pad, src, EVP_sha256(),
                                    -2); /* The given message is padded with RSA-PSS*/
    if (ret <= 0) {
        fatal_error();
    }

    return RSA_private_encrypt(len, msg_pad, out_buf13, rsa,
                               RSA_NO_PADDING); /* Fix: RSA-PSS is a padding
                                                 for signature operations */
}


/*============================================================================
 * RSA NO BLINDING
 *==========================================================================*/
unsigned char* out_buf14;
int bug_cryptorsanoblinding(unsigned char* src, size_t len, RSA* rsa) {
    if (rsa == NULL) {
        fatal_error(); /* The given context is provided */
    }

    RSA_blinding_off(rsa); /* the RSA blinding is turned off */
    return RSA_private_decrypt(len, src, out_buf14, rsa,
                               RSA_PKCS1_OAEP_PADDING); /* Defect: no RSA blinding is set
                                                         for this decryption */
}

int corrected_cryptorsanoblinding(unsigned char* src, size_t len, RSA* rsa) {
    int ret;
    if (rsa == NULL) {
        fatal_error(); /* The given context is provided */
    }

    ret = RSA_blinding_on(rsa, NULL); /* the RSA blinding is turned on */
    if (ret <= 0) {
        fatal_error();
    }

    return RSA_private_decrypt(len, src, out_buf14, rsa,
                               RSA_PKCS1_OAEP_PADDING); /* Fix: RSA blinding is set
                                                         for this decryption */
}


/******************
 * HASH FUNCTIONS *
 ******************/


/*============================================================================
 * MD WEAK HASH
 *==========================================================================*/
unsigned char* out_buf15;
unsigned int out_len15;
void bug_cryptomdweakhash(unsigned char* src, size_t len, EVP_PKEY* pkey) {
    int ret;
    EVP_MD_CTX* ctx = EVP_MD_CTX_create();
    if (!ctx) {
        fatal_error();
    }

    ret = EVP_SignInit_ex(ctx, EVP_md5(), NULL); /* the given hash function is set in the context */
    if (ret != 1) {
        fatal_error();
    }

    ret = EVP_SignUpdate(ctx, src, len); /* Defect: MD5 is a broken hash function,
                                            this computation is not secure enough */
    if (ret != 1) {
        fatal_error();
    }

    ret = EVP_SignFinal(ctx, out_buf15, &out_len15, pkey);
    if (ret != 1) {
        fatal_error();
    }
}

void corrected_cryptomdweakhash(unsigned char* src, size_t len, EVP_PKEY* pkey) {
    int ret;
    EVP_MD_CTX* ctx = EVP_MD_CTX_create();
    if (!ctx) {
        fatal_error();
    }

    ret = EVP_SignInit_ex(ctx, EVP_sha256(),
                          NULL); /* the given hash function is set in the context */
    if (ret != 1) {
        fatal_error();
    }

    ret = EVP_SignUpdate(ctx, src, len); /* Fix: SHA256 is a secure hash function */
    if (ret != 1) {
        fatal_error();
    }

    ret = EVP_SignFinal(ctx, out_buf15, &out_len15, pkey);
    if (ret != 1) {
        fatal_error();
    }
}


/*============================================================================
 * MD BAD FUNCTION
 *==========================================================================*/
unsigned char* out_buf16;
unsigned int out_len16;
void bug_cryptomdbadfunction(unsigned char* src, size_t len) {
    int ret;
    EVP_MD_CTX* ctx = EVP_MD_CTX_create();
    if (!ctx) {
        fatal_error();
    }

    ret = EVP_SignInit_ex(ctx, EVP_sha256(),
                          NULL); /* the given hash function is set in the context */
    if (ret != 1) {
        fatal_error();
    }

    ret = EVP_SignUpdate(ctx, src, len);
    if (ret != 1) {
        fatal_error();
    }

    ret = EVP_DigestSignFinal(ctx, out_buf16,
                              (size_t*)out_len16); /* Defect: the initialized operation in the
                                                    context is not compatible with this function*/
    if (ret != 1) {
        fatal_error();
    }
}

void corrected_cryptomdbadfunction(unsigned char* src, size_t len, EVP_PKEY* pkey) {
    int ret;
    EVP_MD_CTX* ctx = EVP_MD_CTX_create();
    if (!ctx) {
        fatal_error();
    }

    ret = EVP_SignInit_ex(ctx, EVP_sha256(),
                          NULL); /* the given hash function is set in the context */
    if (ret != 1) {
        fatal_error();
    }

    ret = EVP_SignUpdate(ctx, src, len);
    if (ret != 1) {
        fatal_error();
    }

    ret = EVP_SignFinal(ctx, out_buf16, &out_len16, pkey); /* Fix: the called functions are coherent
                                                            with the initialized context */
    if (ret != 1) {
        fatal_error();
    }
}


/*************
 * PROTOCOLS *
 *************/


/*============================================================================
 * SSL WEAK PROTOCOL
 *==========================================================================*/
#if defined(__GNUC__) && !defined(_WIN32) && !defined(__llvm__) /* This compiles on GCC Linux only */
int bug_cryptosslweakprotocol() {
    int ret;
    SSL_CTX* ctx;
    SSL* ssl;

    SSL_library_init(); /* initialize library */

    /* context configuration */
    ctx = SSL_CTX_new(SSLv23_client_method()); /* context object for the SSL protocol */
    if (ctx == NULL) {
        fatal_error();
    }

    ret = SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM); /* the given certificate
                                                                            is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    ret = SSL_CTX_load_verify_locations(ctx, NULL,
                                        "ca/path"); /* the CA location is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    /* Handle connection */
    ssl = SSL_new(ctx);
    if (ssl == NULL) {
        fatal_error();
    }

    SSL_set_fd(ssl, NULL);

    ret = SSL_connect(ssl); /* Defect: When not setting options to restrict protocols,
                              the connection may use weak protocols (SSLv2, SSLv3, TLSv1),
                              this negotiation is not secure enough */
    if (ret <= 0) {
        fatal_error();
    }

    return ret;
}

int corrected_cryptosslweakprotocol() {
    int ret;
    SSL_CTX* ctx;
    SSL* ssl;

    SSL_library_init(); /* initialize library */

    ctx = SSL_CTX_new(SSLv23_client_method()); /* context object for the SSL protocol */
    if (ctx == NULL) {
        fatal_error();
    }

    /* context configuration */
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1);
    ret = SSL_CTX_load_verify_locations(ctx, NULL,
                                        "ca/path"); /* the CA location is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    ret = SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM); /* the given certificate
                                                                            is set in the context */
    if (ret <= 0) {
        fatal_error();
    }

    /* Handle connection */
    ssl = SSL_new(ctx);
    if (ssl == NULL) {
        fatal_error();
    }

    SSL_set_fd(ssl, NULL);

    ret = SSL_connect(ssl); /* Fix: Only secure protocols (TLSv11, TLSv12, ...) are allowed  */
    if (ret <= 0) {
        fatal_error();
    }

    return ret;
}
#endif /* __GNUC__ */
