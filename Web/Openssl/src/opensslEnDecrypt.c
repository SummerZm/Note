#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "openssl/ssl.h"
#include "openssl/ssl23.h"
#include "openssl/err.h"
#include <openssl/aes.h>

#define ZM_AES_CBC_KEY  "8cc72b05705d5c46f412af8cbed55aad"
#define ZM_AES_CBC_IV   "667b02a85c61c786def4521b060265e8"

/* Openssl cbc encrypt/decrypt. This is block en/decrypt. */
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
    unsigned char *key = str2hex(ZM_AES_CBC_KEY);
    unsigned char *iv = str2hex(ZM_AES_CBC_IV);
    AES_set_encrypt_key(key,128,&aes);//注释3
    AES_cbc_encrypt(rawBuf, encrpyBuf,len,&aes,iv,AES_ENCRYPT);
    free(key);
    free(iv);
}

void AES_decrpyt(const char *rawBuf, char *encrpyBuf, int len ) {
    AES_KEY aes;
    unsigned char *key = str2hex(ZM_AES_CBC_KEY);
    unsigned char *iv = str2hex(ZM_AES_CBC_IV);
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

/* Openssl cfb encrypt/decrypt. This is stream en/decrypt. */
static AES_KEY key;
static int aesDecryedNum=0;
static unsigned char ckey[18];
static unsigned char ivec[18];

void HTTP_AES_cfb1_init() {
	memset(ckey, 0, sizeof(ckey));
	memset(ivec, 0, sizeof(ivec));
	memcpy(ivec, "1234567890abcdef", 16);
	memcpy(ckey, "1234567890abcdef", 16);
  	AES_set_encrypt_key(ckey, 128, &key);
}

void HTTP_AES_cfb1_destory() {
	aesDecryedNum= 0;
	memset(ckey, 0, sizeof(ckey));
	memset(ivec, 0, sizeof(ivec));
	memset(&key, 0, sizeof(key));
}

int HTTP_AES_cfb1_crypt(const char* inBuf, const char* outBuf, int len, int crypt) {
	if (inBuf==NULL || outBuf==NULL) {
		return -1;
	}
  	AES_set_encrypt_key(ckey, 128, &key);
	if (crypt==1) {
		AES_cfb128_encrypt(inBuf, outBuf, len, &key, ivec, &aesDecryedNum, AES_ENCRYPT);
	}
	else {
		AES_cfb128_encrypt(inBuf, outBuf, len, &key, ivec, &aesDecryedNum, AES_DECRYPT);
	}
	return 0;
}

int HTTP_AES_cfb1_crpyt_file(const char* origin, const char* outfile, int cryptMode) {
	FILE *fd, *fd1;
	int rBytes, wBytes;
	unsigned char inData[20];
	unsigned char outData[20];
	if (origin==NULL || outfile==NULL) {
		printf("Filename error : origin[%s] - outfile[%s]\n", origin, outfile);
		return -1;
	}
	fd = fopen(origin, "rb+");
	if (fd==NULL) {
		printf("Open %s failed\n", origin);
		return -1;
	}
	fd1 = fopen(outfile, "wb+");
	if (fd1==NULL) {
		printf("Open %s failed\n", outfile);
		fclose(fd);
		return -1;
	}
	
	HTTP_AES_cfb1_init();
	while (!feof(fd)) {
		memset(inData, 0, sizeof(inData));
		memset(outData, 0, sizeof(outData));
		rBytes = fread(inData, 1, sizeof(inData), fd);
		if (rBytes!=sizeof(inData) && ferror(fd)) {
			perror("Read:");
			break;
		}
		HTTP_AES_cfb1_crypt(inData, outData, rBytes, cryptMode);
		wBytes = fwrite(outData, 1, rBytes, fd1);
		if (wBytes!=sizeof(rBytes) && ferror(fd1)) {
			perror("write:");
			break;
		}
	}	
	HTTP_AES_cfb1_destory();
	fclose(fd);
	fflush(fd1);
	fclose(fd1);
	return 0;
}

int HTTP_AES_cfb1_decrypt_file(const char* infile, const char* outfile) {
	HTTP_AES_cfb1_crpyt_file(infile, outfile, 0);
	return 0;
}

int HTTP_AES_cfb1_encrypt_file(const char* infile, const char* outfile) {
	HTTP_AES_cfb1_crpyt_file(infile, outfile, 1);
	return 0;
}

int HTTP_AES_cfb1_crypt_test(char* origin, char* encryptFile, char* decryptFile) {
	HTTP_AES_cfb1_encrypt_file(origin, encryptFile);
	HTTP_AES_cfb1_decrypt_file(encryptFile, decryptFile);
	return 0;
}

int main(int argc, char** argv) {
    SSL_Encrypt_Decrypt_test();
	if (argc==4) {
		HTTP_AES_cfb1_crypt_test(argv[1], argv[2], argv[3]);
	}
    return 0;
}







