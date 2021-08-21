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

void AES_encrpyt_plus(char *rawBuf, char *encrpyBuf, int len, char* kdfKey, int kenLen) {
    AES_KEY aes;
	if (kdfKey != NULL) 
	{
		unsigned char *key = str2hex(kdfKey);
		unsigned char *iv = str2hex(LONBON_AES_CBC_IV);
		AES_set_encrypt_key(key, kenLen*4, &aes);
		AES_cbc_encrypt(rawBuf, encrpyBuf,len,&aes,iv,AES_ENCRYPT);
		free(key);
		free(iv);
	}
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

/* 
 *  AES128 - The Length of kdfkey must be 32.
 */
void AES_decrpyt_plus(char *rawBuf, char *encrpyBuf, int len, char* kdfKey, int kenLen) {
    AES_KEY aes;
	if (kdfKey != NULL) 
	{
		unsigned char *key = str2hex(kdfKey);
		unsigned char *iv = str2hex(LONBON_AES_CBC_IV);
		AES_set_decrypt_key(key, kenLen*4,&aes);
		AES_cbc_encrypt(rawBuf, encrpyBuf,len,&aes,iv,AES_DECRYPT);
		free(key);
		free(iv);
	}
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

/*
 *  if (kdfKey == NULL); Then use defualt mode.
 */
int HTTP_AES_encrpyt_plus(const char *rawBuf, int rawLen, char *encrpyBuf, int encrpyLen, const char* kdfKey, int kenLen) {
    int paddingSize= 0;
    char* paddingBuf = AES_padding_buf(rawBuf, rawLen, &paddingSize);
    //printf_buff(paddingBuf, paddingSize);
    if (paddingSize>encrpyLen) {
        SS_LOG(SS_DEBUG, "Encrpy buff is too small.");
	if (paddingBuf) free(paddingBuf);
        return;
    }
    memset(encrpyBuf, 0, encrpyLen);
	if (kdfKey) {
    	AES_encrpyt_plus(paddingBuf, encrpyBuf, paddingSize, kdfKey, kenLen);
	}
	else {
		char kdfKey[20];
		char key[34];
		memset(kdfKey, 0, sizeof(kdfKey));
		memset(key, 0, sizeof(key));
		SSL_KDF_generate_key(NULL, 0, NULL, 0, 1024, kdfKey, 16);
		str_to_hex(kdfKey, 16, key, sizeof(key));
    	AES_encrpyt_plus(paddingBuf, encrpyBuf, paddingSize, key, 32);
	}

    //printf_buff(encrpyBuf, paddingSize);
    free(paddingBuf);
    return paddingSize;
}

void HTTP_AES_decrpyt(const char *rawBuf, char *decrpyBuf, int decrpyLen) {
    return AES_decrpyt(rawBuf, decrpyBuf, decrpyLen);
}

/*
 *  if (kdfKey == NULL); Then use defualt mode.
 */
void HTTP_AES_decrpyt_plus(const char *rawBuf, char *decrpyBuf, int decrpyLen, const char* kdfKey, int kenLen) {
	if (kdfKey) {
    	return AES_decrpyt_plus(rawBuf, decrpyBuf, decrpyLen, kdfKey, kenLen);
	}
	else {
		char kdfKey[20];
		char key[34];
		memset(kdfKey, 0, sizeof(kdfKey));
		memset(key, 0, sizeof(key));
		SSL_KDF_generate_key(NULL, 0, NULL, 0, 1024, kdfKey, 16);
		str_to_hex(kdfKey, 16, key, sizeof(key));
		AES_decrpyt_plus(rawBuf, decrpyBuf, decrpyLen, key, 32);
	}
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

int HTTP_AES_cfb1_crypt_plus(const char* inBuf, const char* outBuf, int len, int crypt, AES_KEY* kdfKey) {
	if (inBuf==NULL || outBuf==NULL) {
		return -1;
	}
	
	if (kdfKey==NULL) {
		return HTTP_AES_cfb1_crypt(inBuf, outBuf, len, crypt);
	}

	if (crypt==1) {
		AES_cfb128_encrypt(inBuf, outBuf, len, kdfKey, ivec, &aesCryptedNum, AES_ENCRYPT);
	}
	else {
		AES_cfb128_encrypt(inBuf, outBuf, len, kdfKey, ivec, &aesCryptedNum, AES_DECRYPT);
	}
	return 0;
}
int HTTP_AES_cfb1_crypt_file_to_mem(const char* infile, char* buff, int buffLen, int cryptMode, AES_KEY* key) {
	int used = 0;
	FILE *fd = NULL;
	unsigned char inData[20];
	unsigned char outData[20];
	// check 
	if (infile==NULL || buff==NULL) {
		SS_LOG(SS_DEBUG, "Infile or buff is null. do nothing. exit.");
		return 0;
	}
	if (!existfile(infile)) {
		SS_LOG(SS_DEBUG, "Infile %s no exist. do nothing. exit.");
		return 0;
	}

	fd = fopen(infile, "r");
	if (fd==NULL) {
		SS_LOG(SS_DEBUG, "Infile %s open failed. do nothing. exit.");
		return 0;
	}
	HTTP_AES_cfb1_init();
	while (!feof(fd)) {
		memset(inData, 0, sizeof(inData));
		memset(outData, 0, sizeof(outData));
		int rBytes = fread(inData, 1, sizeof(inData), fd);
		if (rBytes!=sizeof(inData) && ferror(fd)) {
			perror("Read:");
			break;
		}
		HTTP_AES_cfb1_crypt_plus(inData, outData, rBytes, cryptMode, key);
		if (buffLen-used>sizeof(outData)) {
			memcpy(buff+used, outData, sizeof(outData));
			used += sizeof(outData);
		}
		else {
			SS_LOG(SS_DEBUG, "Buffsize[%d] is too small. exit", buffLen);
			break;
		}
	}	
	HTTP_AES_cfb1_destory();
	fclose(fd);
	return 0;
}
int HTTP_AES_cfb1_crypt_file(const char* origin, const char* outfile, int cryptMode, AES_KEY* key) {
	FILE *fd, *fd1;
	int rBytes, wBytes;
	unsigned char inData[20];
	unsigned char outData[20];
	if (origin==NULL || outfile==NULL) {
		//printf("Filename error : origin[%s] - outfile[%s]\n", origin, outfile);
		return -1;
	}
	fd = fopen(origin, "rb+");
	if (fd==NULL) {
		//printf("Open %s failed\n", origin);
		return -1;
	}
	fd1 = fopen(outfile, "wb+");
	if (fd1==NULL) {
		//printf("Open %s failed\n", outfile);
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
		HTTP_AES_cfb1_crypt_plus(inData, outData, rBytes, cryptMode, key);
		wBytes = fwrite(outData, 1, rBytes, fd1);
		if (wBytes!=rBytes && ferror(fd1)) {
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

int HTTP_AES_cfb1_decrypt_file_to_mem(const char* infile, char* buff, int buffLen) {
	HTTP_AES_cfb1_crypt_file_to_mem(infile, buff, buffLen, 0, NULL);
}

int HTTP_AES_cfb1_decrypt_file_to_mem_plus(const char* infile, char* buff, int buffLen, AES_KEY* key) {
	if (key) {
		HTTP_AES_cfb1_crypt_file_to_mem(infile, buff, buffLen, 0, key);
	}
	else {
		AES_KEY aes;
		char kdfKey[20];
		char key[34];
		memset(kdfKey, 0, sizeof(kdfKey));
		memset(key, 0, sizeof(key));
		SSL_KDF_generate_key(NULL, 0, NULL, 0, 1024, kdfKey, 16);
		str_to_hex(kdfKey, 16, key, sizeof(key));
		//SS_LOG(SS_DEBUG, "kdfKey:%s", kdfKey);
		//printf("Key:%s\n", key);
    	AES_set_encrypt_key(key,128,&aes);
		HTTP_AES_cfb1_crypt_file_to_mem(infile, buff, buffLen, 0, &aes);
		return 0;
	}
}

int HTTP_AES_cfb1_encrypt_file_to_mem(const char* infile, char* buff, int buffLen) {
	HTTP_AES_cfb1_crypt_file_to_mem(infile, buff, buffLen, 1, NULL);
}

int HTTP_AES_cfb1_encrypt_file_to_mem_plus(const char* infile, char* buff, int buffLen, AES_KEY* key) {
	if (key) {
		HTTP_AES_cfb1_crypt_file_to_mem(infile, buff, buffLen, 1, key);
	}
	else {
		AES_KEY aes;
		char kdfKey[20];
		char key[34];
		memset(kdfKey, 0, sizeof(kdfKey));
		memset(key, 0, sizeof(key));
		SSL_KDF_generate_key(NULL, 0, NULL, 0, 1024, kdfKey, 16);
		str_to_hex(kdfKey, 16, key, sizeof(key));
		//SS_LOG(SS_DEBUG, "kdfKey:%s", kdfKey);
		//printf("Key:%s\n", key);
    	AES_set_encrypt_key(key,128,&aes);
		HTTP_AES_cfb1_crypt_file_to_mem(infile, buff, buffLen, 1, &aes);
		return 0;
	}
}

int HTTP_AES_cfb1_decrypt_file(const char* infile, const char* outfile) {
	HTTP_AES_cfb1_crpyt_file(infile, outfile, 0);
	return 0;
}
int HTTP_AES_cfb1_decrypt_file_plus(const char* infile, const char* outfile, AES_KEY* kdfKey) {
	if (kdfKey) {
		HTTP_AES_cfb1_crypt_file(infile, outfile, 0, kdfKey);
	}
	else {
		AES_KEY aes;
		char kdfKey[20];
		char key[34];
		memset(kdfKey, 0, sizeof(kdfKey));
		memset(key, 0, sizeof(key));
		SSL_KDF_generate_key(NULL, 0, NULL, 0, 1024, kdfKey, 16);
		str_to_hex(kdfKey, 16, key, sizeof(key));
		//SS_LOG(SS_DEBUG, "kdfKey:%s", kdfKey);
		//printf("Key:%s\n", key);
    	AES_set_encrypt_key(key,128,&aes);
		HTTP_AES_cfb1_crypt_file(infile, outfile, 0, &aes);
		return 0;
	}
}

int HTTP_AES_cfb1_encrypt_file(const char* infile, const char* outfile) {
	HTTP_AES_cfb1_crpyt_file(infile, outfile, 1);
	return 0;
}
int HTTP_AES_cfb1_encrypt_file_plus(const char* infile, const char* outfile, AES_KEY* kdfKey) {
	if (kdfKey) {
		return HTTP_AES_cfb1_crypt_file(infile, outfile, 1, kdfKey);
	}
	else {
		AES_KEY aes;
		char kdfKey[20];
		char key[34];
		memset(kdfKey, 0, sizeof(kdfKey));
		memset(key, 0, sizeof(key));
		SSL_KDF_generate_key(NULL, 0, NULL, 0, 1024, kdfKey, 16);
		str_to_hex(kdfKey, 16, key, sizeof(key));
		//SS_LOG(SS_DEBUG, "kdfKey:%s", kdfKey);
		//printf("Key:%s\n", key);
    	AES_set_encrypt_key(key,128,&aes);
		HTTP_AES_cfb1_crypt_file(infile, outfile, 1, &aes);
		return 0;
	}
}

int SSL_KDF_generate_key(const unsigned char *pass, int passLen, const unsigned char *salt, int saltLen, int iter, const unsigned char* kdfKey, int keyLen) 
{
	int ret = 0;
	char passInfo[1028];
	
	if (pass==NULL) {  // Use defualt pass.
		FILE* fd = NULL;
		memset(passInfo, 0, sizeof(passInfo));
		fd = fopen(LONBON_SERISE_FILE, "r");
		if (fd!=NULL) {
			fread(passInfo, 1, sizeof(passInfo),fd);
			fclose(fd);

			pass = passInfo;
			passLen = strlen(passInfo);
		}
		else {
			SS_LOG(SS_DEBUG, "Open serise file failed");
		}	
	}

	if (salt==NULL) { // Use defualt salt.
		salt = LONBON_SERISE_SALT;
		saltLen = strlen(salt);
	}	

	if (pass && salt && kdfKey) {
		memset(kdfKey, 0, keyLen);
		ret = PKCS5_PBKDF2_HMAC_SHA1(pass, passLen, salt, saltLen, iter, keyLen, kdfKey);
	}
	else {
        SS_LOG(SS_DEBUG, "SSL_KDF_generate_key [pass|salt|kdfKey is null] do nothing exit.");
	}
	return ret;
}

int HTTP_AES_cfb1_crypt_test(char* origin, char* encryptFile, char* decryptFile) {
	HTTP_AES_cfb1_encrypt_file(origin, encryptFile);
	HTTP_AES_cfb1_decrypt_file(encryptFile, decryptFile);
	return 0;
}

int SSL_test() {
	{
		char key[34];
		char kdfKey[48];
		memset(key, 0, sizeof(key));
		memset(kdfKey, 0, sizeof(kdfKey));
		
		SSL_KDF_generate_key(NULL, 0, NULL, 0, 1024, kdfKey, 16);
		printf("PBKDF2: %s\n", kdfKey);
		str_to_hex(kdfKey, 16, key, sizeof(key));
		printf("Root Key: %s\n", key);
		printf("===========[TEST 1 END]=============n");
	}

	{
		char* str = "123456";
		char encrpyBuf[64];
		char decrpyBuf[64];
		int paddingSize = 0;	/* Important variable */
		int encrpySize = sizeof(encrpyBuf);
		int decrpySize= sizeof(decrpyBuf);
		memset(encrpyBuf, 0, sizeof(encrpyBuf));
		memset(decrpyBuf, 0, sizeof(decrpyBuf));
		printf("Raw data: %s\n", str);
		paddingSize = HTTP_AES_encrpyt_plus(str, strlen(str), encrpyBuf, encrpySize, NULL, 32);
		printf("Encrypt: %s\n", encrpyBuf);
		HTTP_AES_decrpyt_plus(encrpyBuf, decrpyBuf, paddingSize, NULL, 32);
		printf("Decrypt: %s\n", decrpyBuf);
		printf("===========[TEST 2 END]=============n");
	}

	{
		char buff[9096];
		memset(buff, 0, sizeof(buff));
		HTTP_AES_cfb1_encrypt_file_plus("/usr/local/sbin/server.key",  "/usr/local/sbin/server1.key", NULL);
		HTTP_AES_cfb1_decrypt_file_plus("/usr/local/sbin/server1.key", "/usr/local/sbin/server2.key", NULL);
		HTTP_AES_cfb1_decrypt_file_to_mem_plus("/usr/local/sbin/server1.key", buff, sizeof(buff), NULL);
		printf("\n");
		printf("%s\n", buff);
		printf("\n");
		printf("===========[TEST 3 END]=============n");
	}

}

int main(int argc, char** argv) {
    SSL_Encrypt_Decrypt_test();
	if (argc==4) {
		HTTP_AES_cfb1_crypt_test(argv[1], argv[2], argv[3]);
	}
    return 0;
}







