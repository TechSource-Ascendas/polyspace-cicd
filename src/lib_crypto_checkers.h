/*    Copyright 2016-2025 The MathWorks, Inc.
 *
 * Interface used only in demo of cryptography checkers
 * Contains only non standard functions used in demos
 * These are the declarations of types and functions used in the demo
 * and defined in OpenSSL
 *
 * Interface needed by VULNERABLE_PRNG and CRYPTO_CIPHER_* checkers
 */

#ifndef LIB_CRYPTO_CHECKERS_H
#define LIB_CRYPTO_CHECKERS_H


/* Structure definitions */

struct crypto_cipher;
typedef struct crypto_cipher EVP_CIPHER;

struct crypto_cipher_ctx;
typedef struct crypto_cipher_ctx EVP_CIPHER_CTX;

struct crypto_pkey;
typedef struct crypto_cipher EVP_PKEY;

struct crypto_pkey_ctx;
typedef struct crypto_pkey_ctx EVP_PKEY_CTX;

struct crypto_md;
typedef struct crypto_md EVP_MD;

struct crypto_md_ctx;
typedef struct crypto_md_ctx EVP_MD_CTX;

struct crypto_ssl;
typedef struct crypto_ssl SSL;

struct crypto_ssl_ctx;
typedef struct crypto_ssl_ctx SSL_CTX;

struct crypto_ssl_method;
typedef struct crypto_ssl_method SSL_METHOD;

struct crypto_ssl_verfiy_cb;
typedef struct crypto_ssl_verify_cb SSL_verify_cb;

struct crypto_rsa;
typedef struct crypto_rsa RSA;

struct crypto_dsa;
typedef struct crypto_dsa DSA;

struct crypto_engine;
typedef struct crypto_engine ENGINE;

struct crypto_bignum;
typedef struct crypto_bignum BIGNUM;

struct crypto_bn_ctx;
typedef struct crypto_bn_ctx BN_CTX;

/* Symmetric functions */

void EVP_CIPHER_CTX_init(EVP_CIPHER_CTX* a);
EVP_CIPHER_CTX* EVP_CIPHER_CTX_new(void);
int EVP_CIPHER_CTX_cleanup(EVP_CIPHER_CTX* a);

int EVP_CipherInit_ex(EVP_CIPHER_CTX* ctx,
                      const EVP_CIPHER* cipher,
                      ENGINE* impl,
                      const unsigned char* key,
                      const unsigned char* iv,
                      int enc);

int EVP_DecryptInit_ex(EVP_CIPHER_CTX* ctx,
                       const EVP_CIPHER* cipher,
                       ENGINE* impl,
                       const unsigned char* key,
                       const unsigned char* iv);

int EVP_EncryptInit(EVP_CIPHER_CTX* ctx,
                    const EVP_CIPHER* cipher,
                    const unsigned char* key,
                    const unsigned char* iv);
int EVP_EncryptInit_ex(EVP_CIPHER_CTX* ctx,
                       const EVP_CIPHER* cipher,
                       ENGINE* impl,
                       const unsigned char* key,
                       const unsigned char* iv);
int EVP_EncryptUpdate(EVP_CIPHER_CTX* ctx,
                      unsigned char* out,
                      int* outl,
                      const unsigned char* in,
                      int inl);
int EVP_EncryptFinal_ex(EVP_CIPHER_CTX* ctx, unsigned char* out, int* outl);

const EVP_CIPHER* EVP_des_cbc(void);
const EVP_CIPHER* EVP_aes_128_cbc(void);
const EVP_CIPHER* EVP_aes_128_ecb(void);

int RAND_bytes(unsigned char* buf, int num);
int RAND_pseudo_bytes(unsigned char* buf, int num);

/* Asymmetric functions */

#define EVP_PKEY_OP_UNDEFINED 0
#define EVP_PKEY_OP_PARAMGEN (1 << 1)
#define EVP_PKEY_OP_KEYGEN (1 << 2)
#define EVP_PKEY_OP_SIGN (1 << 3)
#define EVP_PKEY_OP_VERIFY (1 << 4)
#define EVP_PKEY_OP_VERIFYRECOVER (1 << 5)
#define EVP_PKEY_OP_SIGNCTX (1 << 6)
#define EVP_PKEY_OP_VERIFYCTX (1 << 7)
#define EVP_PKEY_OP_ENCRYPT (1 << 8)
#define EVP_PKEY_OP_DECRYPT (1 << 9)
#define EVP_PKEY_OP_DERIVE (1 << 10)

#define EVP_PKEY_CTRL_MD 1
#define EVP_PKEY_CTRL_PEER_KEY 2

#define EVP_PKEY_CTRL_PKCS7_ENCRYPT 3
#define EVP_PKEY_CTRL_PKCS7_DECRYPT 4

#define EVP_PKEY_CTRL_PKCS7_SIGN 5

#define EVP_PKEY_CTRL_SET_MAC_KEY 6

#define EVP_PKEY_CTRL_DIGESTINIT 7

#define EVP_PKEY_ALG_CTRL 0x1000

#define EVP_PKEY_OP_TYPE_SIG                                                                   \
    (EVP_PKEY_OP_SIGN | EVP_PKEY_OP_VERIFY | EVP_PKEY_OP_VERIFYRECOVER | EVP_PKEY_OP_SIGNCTX | \
     EVP_PKEY_OP_VERIFYCTX)

/*
 * Method handles all operations: don't assume any digest related defaults.
 */

#define EVP_PKEY_RSA 1


#define EVP_PKEY_CTRL_RSA_PADDING (EVP_PKEY_ALG_CTRL + 1)
#define EVP_PKEY_CTRL_RSA_PSS_SALTLEN (EVP_PKEY_ALG_CTRL + 2)

#define EVP_PKEY_CTRL_RSA_KEYGEN_BITS (EVP_PKEY_ALG_CTRL + 3)
#define EVP_PKEY_CTRL_RSA_KEYGEN_PUBEXP (EVP_PKEY_ALG_CTRL + 4)
#define EVP_PKEY_CTRL_RSA_MGF1_MD (EVP_PKEY_ALG_CTRL + 5)

#define EVP_PKEY_CTRL_GET_RSA_PADDING (EVP_PKEY_ALG_CTRL + 6)
#define EVP_PKEY_CTRL_GET_RSA_PSS_SALTLEN (EVP_PKEY_ALG_CTRL + 7)
#define EVP_PKEY_CTRL_GET_RSA_MGF1_MD (EVP_PKEY_ALG_CTRL + 8)

#define EVP_PKEY_CTRL_RSA_OAEP_MD (EVP_PKEY_ALG_CTRL + 9)
#define EVP_PKEY_CTRL_RSA_OAEP_LABEL (EVP_PKEY_ALG_CTRL + 10)

#define EVP_PKEY_CTRL_GET_RSA_OAEP_MD (EVP_PKEY_ALG_CTRL + 11)
#define EVP_PKEY_CTRL_GET_RSA_OAEP_LABEL (EVP_PKEY_ALG_CTRL + 12)

#define RSA_PKCS1_PADDING 1
#define RSA_SSLV23_PADDING 2
#define RSA_NO_PADDING 3
#define RSA_PKCS1_OAEP_PADDING 4
#define RSA_X931_PADDING 5
/* EVP_PKEY_ only */
#define RSA_PKCS1_PSS_PADDING 6

#define RSA_PKCS1_PADDING_SIZE 11


/* macro definitions : */
#define EVP_PKEY_CTX_set_signature_md(ctx, md) \
    EVP_PKEY_CTX_ctrl(ctx, -1, EVP_PKEY_OP_TYPE_SIG, EVP_PKEY_CTRL_MD, 0, (void*)md)

#define EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits)                                               \
    EVP_PKEY_CTX_ctrl(ctx, EVP_PKEY_RSA, EVP_PKEY_OP_KEYGEN, EVP_PKEY_CTRL_RSA_KEYGEN_BITS, bits, \
                      NULL)

#define EVP_PKEY_CTX_set_rsa_keygen_pubexp(ctx, pubexp)                                          \
    EVP_PKEY_CTX_ctrl(ctx, EVP_PKEY_RSA, EVP_PKEY_OP_KEYGEN, EVP_PKEY_CTRL_RSA_KEYGEN_PUBEXP, 0, \
                      pubexp)

#define EVP_PKEY_CTX_set_rsa_padding(ctx, pad) \
    EVP_PKEY_CTX_ctrl(ctx, EVP_PKEY_RSA, -1, EVP_PKEY_CTRL_RSA_PADDING, pad, NULL)

/* EVP_PKEY, EVP_PKEY_CTX functions */

EVP_PKEY* EVP_PKEY_new(void);
EVP_PKEY_CTX* EVP_PKEY_CTX_new(EVP_PKEY* pkey, ENGINE* impl);
EVP_PKEY_CTX* EVP_PKEY_CTX_new_id(int id, ENGINE* impl);
void EVP_PKEY_CTX_free(EVP_PKEY_CTX* ctx);

int EVP_PKEY_encrypt_init(EVP_PKEY_CTX* ctx);
int EVP_PKEY_decrypt_init(EVP_PKEY_CTX* ctx);
int EVP_PKEY_verify_init(EVP_PKEY_CTX* ctx);
int EVP_PKEY_derive_init(EVP_PKEY_CTX* ctx);
int EVP_PKEY_keygen_init(EVP_PKEY_CTX* ctx);

int EVP_PKEY_encrypt(EVP_PKEY_CTX* ctx,
                     unsigned char* out,
                     size_t* out_len,
                     unsigned char* in,
                     size_t in_len);
int EVP_PKEY_decrypt(EVP_PKEY_CTX* ctx,
                     unsigned char* out,
                     size_t* out_len,
                     unsigned char* in,
                     size_t in_len);
int EVP_PKEY_verify(EVP_PKEY_CTX* ctx,
                    const unsigned char* sig,
                    size_t siglen,
                    const unsigned char* tbs,
                    size_t tbslen);
int EVP_PKEY_derive(EVP_PKEY_CTX* ctx, unsigned char* secret, size_t* secret_len);
int EVP_PKEY_keygen(EVP_PKEY_CTX* ctx, EVP_PKEY** pkey);

int EVP_PKEY_derive_set_peer(EVP_PKEY_CTX* ctx, EVP_PKEY* peer);

int EVP_PKEY_CTX_ctrl(EVP_PKEY_CTX* ctx, int keytype, int optype, int cmd, int p1, void* p2);

int EVP_PKEY_set1_RSA(EVP_PKEY* pkey, RSA* key);
int EVP_PKEY_set1_DSA(EVP_PKEY* pkey, DSA* key);

/* RSA functions */
RSA* RSA_new(void);

/* BIGNUM functions */

BIGNUM* BN_new(void);

int BN_set_word(BIGNUM* a, unsigned long w);

/* low level: RSA functions */
int RSA_blinding_on(RSA* rsa, BN_CTX* ctx);
void RSA_blinding_off(RSA* rsa);

int RSA_private_decrypt(int flen,
                        const unsigned char* from,
                        unsigned char* to,
                        RSA* rsa,
                        int padding);
int RSA_private_encrypt(int flen,
                        const unsigned char* from,
                        unsigned char* to,
                        RSA* rsa,
                        int padding);
int RSA_public_encrypt(int flen,
                       const unsigned char* from,
                       unsigned char* to,
                       RSA* rsa,
                       int padding);

int RSA_padding_add_PKCS1_PSS(RSA* rsa,
                              unsigned char* EM,
                              const unsigned char* mHash,
                              const EVP_MD* Hash,
                              int sLen);

/* EVP_MD functions */

#define EVP_SignInit_ex(a, b, c) EVP_DigestInit_ex(a, b, c)
#define EVP_SignInit(a, b) EVP_DigestInit(a, b)
#define EVP_SignUpdate(a, b, c) EVP_DigestUpdate(a, b, c)
#define EVP_VerifyInit_ex(a, b, c) EVP_DigestInit_ex(a, b, c)
#define EVP_VerifyInit(a, b) EVP_DigestInit(a, b)
#define EVP_VerifyUpdate(a, b, c) EVP_DigestUpdate(a, b, c)
#define EVP_OpenUpdate(a, b, c, d, e) EVP_DecryptUpdate(a, b, c, d, e)
#define EVP_SealUpdate(a, b, c, d, e) EVP_EncryptUpdate(a, b, c, d, e)
#define EVP_DigestSignUpdate(a, b, c) EVP_DigestUpdate(a, b, c)
#define EVP_DigestVerifyUpdate(a, b, c) EVP_DigestUpdate(a, b, c)

const EVP_MD* EVP_sha256(void);
const EVP_MD* EVP_md5(void);

EVP_MD_CTX* EVP_MD_CTX_create(void);

int EVP_DigestInit_ex(EVP_MD_CTX* ctx, const EVP_MD* type, ENGINE* impl);
int EVP_DigestUpdate(EVP_MD_CTX* ctx, const void* d, size_t cnt);
int EVP_DigestFinal_ex(EVP_MD_CTX* ctx, unsigned char* md, unsigned int* s);

int EVP_DigestInit(EVP_MD_CTX* ctx, const EVP_MD* type);
int EVP_DigestFinal(EVP_MD_CTX* ctx, unsigned char* md, unsigned int* s);

int EVP_SignFinal(EVP_MD_CTX* ctx, unsigned char* sig, unsigned int* s, EVP_PKEY* pkey);

int EVP_DigestSignFinal(EVP_MD_CTX* ctx, unsigned char* sig, size_t* siglen);

int EVP_DigestVerifyInit(EVP_MD_CTX* ctx,
                         EVP_PKEY_CTX** pctx,
                         const EVP_MD* type,
                         ENGINE* e,
                         EVP_PKEY* pkey);

/* SSL functions */

#define X509_FILETYPE_PEM 1
#define X509_FILETYPE_ASN1 2

#define SSL_CTRL_OPTIONS 32

#define SSL_OP_NO_SSLv2 0x01000000L
#define SSL_OP_NO_SSLv3 0x02000000L
#define SSL_OP_NO_TLSv1 0x04000000L
#define SSL_OP_NO_TLSv1_2 0x08000000L
#define SSL_OP_NO_TLSv1_1 0x10000000L

#define SSL_FILETYPE_ASN1 X509_FILETYPE_ASN1
#define SSL_FILETYPE_PEM X509_FILETYPE_PEM

#define SSL_CTX_set_options(ctx, op) SSL_CTX_ctrl((ctx), SSL_CTRL_OPTIONS, (op), NULL)

int SSL_library_init(void);

const SSL_METHOD* SSLv23_client_method(void);
const SSL_METHOD* SSLv23_server_method(void);
const SSL_METHOD* SSLv23_method(void);
const SSL_METHOD* SSLv2_client_method(void);
const SSL_METHOD* SSLv2_server_method(void);
const SSL_METHOD* SSLv2_method(void);
const SSL_METHOD* SSLv3_client_method(void);
const SSL_METHOD* SSLv3_server_method(void);
const SSL_METHOD* SSLv3_method(void);
const SSL_METHOD* TLSv1_client_method(void);
const SSL_METHOD* TLSv1_server_method(void);
const SSL_METHOD* TLSv1_method(void);
const SSL_METHOD* TLSv1_1_client_method(void);
const SSL_METHOD* TLSv1_1_server_method(void);
const SSL_METHOD* TLSv1_1_method(void);
const SSL_METHOD* TLSv1_2_client_method(void);
const SSL_METHOD* TLSv1_2_server_method(void);
const SSL_METHOD* TLSv1_2_method(void);

SSL_CTX* SSL_CTX_new(const SSL_METHOD* meth);
SSL* SSL_new(SSL_CTX* ctx);

long SSL_CTX_ctrl(SSL_CTX* ctx, int cmd, long larg, void* parg);
long SSL_ctrl(SSL* ssl, int cmd, long larg, void* parg);

int SSL_CTX_load_verify_locations(SSL_CTX* ctx, const char* CAfile, const char* CApath);
void SSL_CTX_set_verify(SSL_CTX* ctx, int mode, SSL_verify_cb verify_callback);
void SSL_set_verify(SSL* s, int mode, SSL_verify_cb verify_callback);
int SSL_CTX_use_certificate_file(SSL_CTX* ctx, const char* file, int type);
int SSL_set_fd(SSL* ssl, int fd);

int SSL_connect(SSL* ssl);
int SSL_accept(SSL* ssl);

#endif /* LIB_CRYPTO_CHECKERS_H */
