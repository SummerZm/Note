#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "openssl/ssl.h"
#include "openssl/ssl23.h"
#include "openssl/err.h"
#include <openssl/aes.h>

#define ZM_RSA_KEY  "8cc72b05705d5c46f412af8cbed55aad"
#define ZM_RSA_IV   "667b02a85c61c786def4521b060265e8"

void printf_buff(const char *buff,int size) {
    int i = 0;
    for (i=0;i<size;i ++ ) {
        printf( "%02X ", (unsigned char)buff[i] );
        if ((i+1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("\n\n");
}

char *AES_padding_buf(const char *buf, int size, int *finalSize) {
    char *ret = NULL;
    int pidding_size = AES_BLOCK_SIZE - (size % AES_BLOCK_SIZE);
    int i;
    *finalSize = size + pidding_size;
    ret = (char *)malloc(size+pidding_size);
    memcpy( ret, buf, size);
    if (pidding_size!=0) {
        for (i =size;i < (size+pidding_size); i++ ) {
            ret[i] = 0;
        }
    }
    return ret;
}

unsigned char* str2hex(char *str) {
    unsigned char *ret = NULL;
    int str_len = strlen(str);
    int i = 0;
    assert((str_len%2) == 0);
    ret = (char *)malloc(str_len/2);
    for (i =0;i < str_len; i = i+2 ) {
        sscanf(str+i,"%2hhx",&ret[i/2]);
    }
    return ret;
}

void AES_encrpyt(char *rawBuf, char *encrpyBuf, int len ) {
    AES_KEY aes;
    unsigned char *key = str2hex(ZM_RSA_KEY);
    unsigned char *iv = str2hex(ZM_RSA_IV);
    AES_set_encrypt_key(key,128,&aes);//注释3
    AES_cbc_encrypt(rawBuf, encrpyBuf,len,&aes,iv,AES_ENCRYPT);
    free(key);
    free(iv);
}

void AES_decrpyt(const char *rawBuf, char *encrpyBuf, int len ) {
    AES_KEY aes;
    unsigned char *key = str2hex(ZM_RSA_KEY);
    unsigned char *iv = str2hex(ZM_RSA_IV);
    AES_set_decrypt_key(key,128,&aes);
    AES_cbc_encrypt(rawBuf, encrpyBuf,len,&aes,iv,AES_DECRYPT);
    free(key);
    free(iv);
}

int HTTP_AES_encrpyt(const char *rawBuf, int rawLen, char *encrpyBuf, int encrpyLen) {
    int paddingSize= 0;
    char* paddingBuf = AES_padding_buf(rawBuf, rawLen, &paddingSize);
    printf_buff(paddingBuf, paddingSize);
    if (paddingSize>encrpyLen) {
        printf("Encrpy buff is too small.");
        return 0;
    }
    memset(encrpyBuf, 0, encrpyLen);
    AES_encrpyt(paddingBuf, encrpyBuf, paddingSize);
    printf_buff(encrpyBuf, paddingSize);
    free(paddingBuf);
    return paddingSize;
}

void HTTP_AES_decrpyt(const char *rawBuf, char *decrpyBuf, int decrpyLen) {
    return AES_decrpyt(rawBuf, decrpyBuf, decrpyLen);
}

void SSL_Encrypt_Decrypt_test() {
     const char* data = "admin";
     char encrpyBuf[256];
     char decrpyBuf[256];
     int encrpySize = sizeof(encrpyBuf);
     int decrpySize = sizeof(decrpyBuf);
     int paddingSize = 0;

     memset(encrpyBuf, 0, sizeof(encrpyBuf));
     memset(decrpyBuf, 0, sizeof(decrpyBuf));
     printf("data:%s\n", data);
     printf_buff(data, 6);
     printf(">>>>>>>>>>>>> EncrpyBuf <<<<<<<<<<<<\n");
     paddingSize =  HTTP_AES_encrpyt(data, strlen(data), encrpyBuf, encrpySize);
     printf("encrpyBuf:%s\n", encrpyBuf);
     printf("\n>>>>>>>>>>>>> DecrpyBuf <<<<<<<<<<<<\n");
     HTTP_AES_decrpyt(encrpyBuf, decrpyBuf, paddingSize);
     printf_buff(decrpyBuf, paddingSize);
     printf("decrpyBuf:%s\n\n", decrpyBuf);
     return;
}

int main(int argc, char** argv) {
    SSL_Encrypt_Decrypt_test();
    return 0;
}







