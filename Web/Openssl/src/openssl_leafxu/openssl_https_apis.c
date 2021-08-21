#include "openssl/ssl.h"
#include "openssl/err.h"
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
 
int SSL_channel_init(SSL_CTX** ctx, const char* certPath, const char* pathvateKeyPath) {
    int ret = -1;
    if (!isTLSEnable() || ctx==NULL || certPath==NULL || pathvateKeyPath==NULL 
        && !existfile(certPath) && !existfile(pathvateKeyPath)) {
        return ret;
    } 
	SS_LOG(SS_DEBUG, "certPath:%s; pathvateKeyPath:%s", certPath, pathvateKeyPath);
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    *ctx = SSL_CTX_new(TLSv1_2_server_method());
    if (*ctx == NULL) {
        ERR_print_errors_fp(stdout);
        SS_LOG(SS_DEBUG, "SSL new ctx failed");
        return ret;
    }

    if (SSL_CTX_use_certificate_file(*ctx, certPath, 1) <= 0) {
        ERR_print_errors_fp(stdout);
        SS_LOG(SS_DEBUG, "SSL use certificate invalid");
        return ret;
    }

	{
		/* Decrypt primary-key file to memory */
		char rsaPrimaryKey[8192];
		memset(rsaPrimaryKey, 0, sizeof(rsaPrimaryKey));
        HTTP_AES_cfb1_decrypt_file_to_mem_plus(pathvateKeyPath, rsaPrimaryKey, sizeof(rsaPrimaryKey), NULL);

		{
			/* Translate primary-key to openssl EVP-KEY object. */
			EVP_PKEY* pkey = NULL;
			BIO* in = BIO_new(BIO_s_mem());
			BIO_write(in, rsaPrimaryKey, strlen(rsaPrimaryKey));
			pkey = PEM_read_bio_PrivateKey(in, NULL, NULL, NULL);
			/* Set primary-key */
			if (SSL_CTX_use_PrivateKey(*ctx, pkey)) {
				ERR_print_errors_fp(stdout);
				SS_LOG(SS_DEBUG, "SSL use privatekey invalid");
				return ret;
			}
			if (pkey) EVP_PKEY_free(pkey);
		}
	}

    if (!SSL_CTX_check_private_key(*ctx)) {
        ERR_print_errors_fp(stdout);
        SS_LOG(SS_DEBUG, "SSL check privatekey invalid");
        return ret;
    }
    return 0;
}

int SSL_session_init(SSL_CTX* ctx, SslSession* session) {
    int ret = -1;
    if (isTLSEnable() && ctx!=NULL && session!=NULL) {
        session->ssl=SSL_new(ctx);
        if (session->ssl==NULL) {
            SS_LOG(SS_DEBUG, "SSL_new failed.");
            return ret;
        }
        session->ctx = ctx;
        session->status = ACCEPTING;
    }
    return 0;
}

void SSL_sesion_destory(SslSession* session) {
    int ret=-1;
    if (!isTLSEnable() || session ==NULL ) {
        return ret;
    }
    SSL_shutdown(session->ssl);
    SSL_free(session->ssl);
    return 0;
}

void SSL_destory(SSL_CTX *ctx) {if (ctx!=NULL && isTLSEnable()) SSL_CTX_free(ctx); }


int SSL_error(SslSession *session, int flag) {
    int ret = -1;
    if (!isTLSEnable()) {return ret;}
    if (session==NULL) {
        SS_LOG(SS_DEBUG, "session is null");
        return ret;
    }
    ret = SSL_get_error(session->ssl, flag);
    switch (ret) {
        case SSL_ERROR_NONE:
        {
            SS_LOG(SS_DEBUG, "SSL_ERROR_NONE");
            break; 
        } 
        case SSL_ERROR_ZERO_RETURN:
        {
            SS_LOG(SS_DEBUG, "SSL_ERROR_ZERO_RETURN");
            break; 
        } 
        case SSL_ERROR_WANT_READ:
        {
            SS_LOG(SS_DEBUG, "SSL_ERROR_WANT_READ");
            break; 
        } 
        case SSL_ERROR_WANT_WRITE:
        {
            SS_LOG(SS_DEBUG, "SSL_ERROR_WANT_WRITE");
            break; 
        } 
        case SSL_ERROR_WANT_CONNECT:
        {
            SS_LOG(SS_DEBUG, "SSL_ERROR_WANT_CONNECT");
            break; 
        } 
        case SSL_ERROR_WANT_ACCEPT:
        {
            SS_LOG(SS_DEBUG, "SSL_ERROR_WANT_ACCEPT");
            break; 
        } 
        case SSL_ERROR_WANT_X509_LOOKUP:
        {
            SS_LOG(SS_DEBUG, "SSL_ERROR_WANT_X509_LOOKUP");
            break; 
        } 
        case SSL_ERROR_SYSCALL:
        {
            SS_LOG(SS_DEBUG, "SSL_ERROR_SYSCALL");
            break; 
        } 
        case SSL_ERROR_SSL:
        {
            SS_LOG(SS_DEBUG, "SSL_ERROR_SSL");
            break; 
        }	
		default:
		{
            SS_LOG(SS_DEBUG, "SSL_ERROR num: %d", ret);
            break; 
		} 
    }   
    return ret;
}

int SSL_session_attach(SSL_CTX* ctx, SslSession* session, int sockfd) {
    int ret = -1;
    if (!isTLSEnable() || session==NULL) {
        return ret;
    }

    if (session->ssl==NULL) {
        //SS_LOG(SS_DEBUG, ">>>>>>>SSL_session_attach:%d<<<<<<<<<<", sockfd);
        SSL_session_init(ctx, session);
        SSL_set_fd(session->ssl, sockfd);
    } 

	ERR_clear_error();
    ret = SSL_accept(session->ssl);
    if (ret == -1) {
		int err = 0;
		if (SSL_error(session, ret) == SSL_ERROR_ZERO_RETURN) {
        	SS_LOG(SS_DEBUG, ">>>>>>>SSL_session_attach: Certificate Unknow[%d]<<<<<<<<<<",ret);
			//SSL_sesion_destory(session);
			return -2;
		}
		else {
        	// SS_LOG(SS_DEBUG, ">>>>>>>SSL_session_attach: ACCEPTING %d<<<<<<<<<<",ret);
        	session->status = ACCEPTING;
		}
        return ret;
    }   
    else {
        //SS_LOG(SS_DEBUG, ">>>>>>>SSL_session_attach:CONNECTED<<<<<<<<<<");
        session->status = CONNECTED;
    }
    return 0;
}

/* Openssl 源码 
*  https://www.openssl.net.cn/
*  https://blog.csdn.net/u012023606/article/details/108407731  
*/
int SSL_CTX_use_PrivateKey_file(SSL_CTX *ctx, const char *file, int type)
{
    int j, ret = 0;
    BIO *in;
    EVP_PKEY *pkey = NULL;
 
    in = BIO_new(BIO_s_file_internal());
    if (in == NULL) {
        SSLerr(SSL_F_SSL_CTX_USE_PRIVATEKEY_FILE, ERR_R_BUF_LIB);
        goto end;
    }
 
    if (BIO_read_filename(in, file) <= 0) {
        SSLerr(SSL_F_SSL_CTX_USE_PRIVATEKEY_FILE, ERR_R_SYS_LIB);
        goto end;
    }
    if (type == SSL_FILETYPE_PEM) {
        j = ERR_R_PEM_LIB;
        pkey = PEM_read_bio_PrivateKey(in, NULL,
                                       ctx->default_passwd_callback,
                                       ctx->default_passwd_callback_userdata);
    } else if (type == SSL_FILETYPE_ASN1) {
        j = ERR_R_ASN1_LIB;
        pkey = d2i_PrivateKey_bio(in, NULL);
    } else {
        SSLerr(SSL_F_SSL_CTX_USE_PRIVATEKEY_FILE, SSL_R_BAD_SSL_FILETYPE);
        goto end;
    }
    if (pkey == NULL) {
        SSLerr(SSL_F_SSL_CTX_USE_PRIVATEKEY_FILE, j);
        goto end;
    }
    ret = SSL_CTX_use_PrivateKey(ctx, pkey);
    EVP_PKEY_free(pkey);
 end:
    if (in != NULL)
        BIO_free(in);
    return (ret);
}

int SSL_CTX_use_PrivateKey(SSL_CTX *ctx, EVP_PKEY *pkey)
{
    if (pkey == NULL) {
        SSLerr(SSL_F_SSL_CTX_USE_PRIVATEKEY, ERR_R_PASSED_NULL_PARAMETER);
        return (0);
    }
    if (!ssl_cert_inst(&ctx->cert)) {
        SSLerr(SSL_F_SSL_CTX_USE_PRIVATEKEY, ERR_R_MALLOC_FAILURE);
        return (0);
    }
    return (ssl_set_pkey(ctx->cert, pkey));
}

static int ssl_set_pkey(CERT *c, EVP_PKEY *pkey)
{
    int i;
    /*
     * Special case for DH: check two DH certificate types for a match. This
     * means for DH certificates we must set the certificate first.
     */
    if (pkey->type == EVP_PKEY_DH) {
        X509 *x;
        i = -1;
        x = c->pkeys[SSL_PKEY_DH_RSA].x509;
        if (x && X509_check_private_key(x, pkey))
            i = SSL_PKEY_DH_RSA;
        x = c->pkeys[SSL_PKEY_DH_DSA].x509;
        if (i == -1 && x && X509_check_private_key(x, pkey))
            i = SSL_PKEY_DH_DSA;
        ERR_clear_error();
    } else
        i = ssl_cert_type(NULL, pkey);
    if (i < 0) {
        SSLerr(SSL_F_SSL_SET_PKEY, SSL_R_UNKNOWN_CERTIFICATE_TYPE);
        return (0);
    }
 
    if (c->pkeys[i].x509 != NULL) {
        EVP_PKEY *pktmp;
        pktmp = X509_get_pubkey(c->pkeys[i].x509);
        if (pktmp == NULL) {
            SSLerr(SSL_F_SSL_SET_PKEY, ERR_R_MALLOC_FAILURE);
            EVP_PKEY_free(pktmp);
            return 0;
        }
        /*
         * The return code from EVP_PKEY_copy_parameters is deliberately
         * ignored. Some EVP_PKEY types cannot do this.
         */
        EVP_PKEY_copy_parameters(pktmp, pkey);
        EVP_PKEY_free(pktmp);
        ERR_clear_error();
 
#ifndef OPENSSL_NO_RSA
        /*
         * Don't check the public/private key, this is mostly for smart
         * cards.
         */
        if ((pkey->type == EVP_PKEY_RSA) &&
            (RSA_flags(pkey->pkey.rsa) & RSA_METHOD_FLAG_NO_CHECK)) ;
        else
#endif
        if (!X509_check_private_key(c->pkeys[i].x509, pkey)) {
            X509_free(c->pkeys[i].x509);
            c->pkeys[i].x509 = NULL;
            return 0;
        }
    }
 
    if (c->pkeys[i].privatekey != NULL)
        EVP_PKEY_free(c->pkeys[i].privatekey);
    CRYPTO_add(&pkey->references, 1, CRYPTO_LOCK_EVP_PKEY);
    c->pkeys[i].privatekey = pkey;
    c->key = &(c->pkeys[i]);
 
    c->valid = 0;
    return (1);
}

/* openssl 交换秘钥 
*  https://www.jishufanyi.cn/287053.html
* */
/* 若具有原始公共密钥，则必须使用正确的参数创建EC_Key。*/
EVP_PKEY * get_peerkey(const unsigned char * buffer, size_t buffer_len)
{
    EC_KEY *tempEcKey = NULL;
    EVP_PKEY *peerkey = NULL;

    // change this if another curve is required
    tempEcKey = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if(tempEcKey == NULL) {
        handleErrors();
    }

    if(EC_KEY_oct2key(tempEcKey, buffer, buffer_len, NULL) != 1)  {
        handleErrors();
    }

    if(EC_KEY_check_key(tempEcKey) != 1) {
        handleErrors();
    }

    peerkey = EVP_PKEY_new();
    if(peerkey == NULL) {
        handleErrors();
    }

    if(EVP_PKEY_assign_EC_KEY(peerkey, tempEcKey)!= 1) {
        handleErrors();
    }

    return peerkey;
}
/* 若将公钥作为ASN.1序列，则可以使用内部转换方法*/
EVP_PKEY* get_peerkey(const unsigned char *buffer, size_t buffer_len)
{
    EVP_PKEY *peerkey = NULL;       
    const unsigned char *helper = buffer;

    // from "openssl/x509.h"
    peerkey = d2i_PUBKEY(NULL, &helper, buffer_len);
    if (!peerkey) {
        handleErrors();
        return NULL;
    }

    return peerkey;
}

/* Openssl 内部格式转换 BIO 接口 【可以将字符串转换成指定openssl内部成员对象 | 数据格式转换】
 * https://blog.csdn.net/liao20081228/article/details/77193729
 */



