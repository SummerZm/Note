#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>


void printf_buff(unsigned char* buff, int len);
int SSL_append_sha512_to_file(char* filename);
int SSL_recovery_sha512_file(char* filename, unsigned char* o_shamd, int len);
int SSL_get_sha512_from_file(char* filename, unsigned char* o_shamd, int len);
int SSL_calcu_file_sha512(char* filename, unsigned char* o_shamd, int len);


void printf_buff(unsigned char* buff, int len)
{
	int i = 0 ;
	if (buff==NULL || len<=0) {
		printf("printf_buff: buff[%s] is null, len [%d]\n", buff, len);
		return;
	}
	for(i=0;i<len;i<i++)
	{
		printf("%02X",buff[i]);
	}
	printf("\n");
	return;
}

int SSL_append_sha512_to_file(char* filename)
{
	unsigned char outSha[20];
	if (filename==NULL) {
		printf("SSL_append_sha512_to_file: filename [%s] is null\n", filename);
		return 0;
	}
	{
		memset(outSha, 0, sizeof(outSha));
		if (0==SSL_calcu_file_sha512(filename, outSha, sizeof(outSha))) {
			return 0;
		}
		printf_buff(outSha, 20);
	}
	{
		FILE* fp = NULL;
		int writeBtyes = 0;
		fp = fopen(filename, "a+");
		if (fp == NULL) {
			printf("SSL_append_sha512_to_file: Open file[%s] no open.\n", filename);
			return 0;
		}
		writeBtyes = fwrite(outSha, 1, sizeof(outSha), fp);
		if (writeBtyes!=sizeof(outSha)) {
			printf("SSL_append_sha512_to_file: write count no equal. writeBtyes[%d]%d\n", writeBtyes, outSha);
			fclose(fp);
			return 0;
		}
		fclose(fp);
	}
	return 1;
}

int SSL_recovery_sha512_file(char* filename, unsigned char* o_shamd, int len)
{
	if (filename==NULL) {
		printf("SSL_get_sha512_from_file: filename is null. Do nothing. Exist!\n");
		return 0;
	}
	if (o_shamd==NULL || len<20) {
		printf("SSL_get_sha512_from_file: outSha[%s] is null or len less then 20[%d]\n", o_shamd, len);
		return 0;
	}
	{
		FILE* fp = NULL;
		int readBytes;
		long filesize = 0;
		memset(o_shamd, 0, len);
		if (0==SSL_get_sha512_from_file(filename, o_shamd, len)) {
			return 0;
		}
		fp = fopen(filename, "rb+");
		if (fp == NULL) {
			printf("SSL_recovery_sha512_file: Open filename[%s] failed\n", filename);
			return 0;
		}
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, filesize-len, SEEK_SET);
		ftruncate(fp, filesize-len);
		fclose(fp);
	}
	return 0;
	
}

int SSL_get_sha512_from_file(char* filename, unsigned char* o_shamd, int len) {
	if (filename==NULL) {
		printf("SSL_get_sha512_from_file: filename is null. Do nothing. Exist!\n");
		return 0;
	}
	if (o_shamd==NULL || len<20) {
		printf("SSL_get_sha512_from_file: outSha[%s] is null or len less then 20[%d]\n", o_shamd, len);
		return 0;
	}
	{
		int readBytes, i;
		long filesize = 0;
		FILE* fp = fopen(filename, "rb");
		if (fp==NULL) {
			printf("SSL_get_sha512_from_file: Open file[%s] failed\n", filename);
			return 0;
		}
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, filesize-len, SEEK_SET);
		memset(o_shamd, 0, len);
		readBytes = fread(o_shamd, 1, len, fp);
		if (readBytes == len) {
			printf("SSL_get_sha512_from_file [sha]: \n");
			printf_buff(o_shamd, 20);
		}
		else {
			printf("SSL_get_sha512_from_file: readBytes[%d] len[%d]\m", readBytes, len);
		}
		fclose(fp);
	}
	return 1;
}

int SSL_calcu_file_sha512(char* filename, unsigned char* o_shamd, int len) {
	if (filename==NULL) {
		printf("filename is null. Do nothing. Exist!\n");
		return 0;
	}

	if (o_shamd==NULL || len<20) {
		printf("outSha[%s] is null or len less then 20[%d]\n", o_shamd, len);
		return 0;
	}
	{
    	SHA512_CTX stx;
    	unsigned char buffer[1024];
    	int readBytes = 0;
    	int i;
    	int n=0;
    	FILE * fp=NULL;
		unsigned char outSha[1024];
	
    	memset(outSha,0,sizeof(outSha));
    	memset(buffer,0,sizeof(buffer));
    	fp=fopen(filename,"rb");
		if (fp==NULL) {
			printf("Open file[%s] faild. Do nothing Exist.\n", filename);
			return 0;
		}

    	SHA512_Init(&stx);
		while((len=fread(buffer,1,1024,fp))>0)
		{
			SHA512_Update(&stx,buffer,len);
			memset(buffer,0,sizeof(buffer));
		}
		SHA512_Final(outSha, &stx);
		memcpy(o_shamd, outSha, 20);
		printf_buff(o_shamd, 20);
		printf("\n");
		fclose(fp);
	}
    return 1;
}

int SSL_Hash_file_test(char* filename)
{
	printf(">>>>>>>>>>>>>>> Test start [SSL_calcu_file_sha512] <<<<<<<<<<<<<<<<<\n");
	{
		unsigned char outSha[20];
		memset(outSha, 0, sizeof(outSha));
		SSL_calcu_file_sha512(filename, outSha, sizeof(outSha));
	}
	printf("Test end [SSL_calcu_file_sha512]\n\n");

	printf(">>>>>>>>>>>>>>> Test start [SSL_append_sha512_to_file] <<<<<<<<<<<<<<<\n");
	{
		SSL_append_sha512_to_file(filename);
	}
	printf("Test end [SSL_append_sha512_to_file]\n\n");


	printf(">>>>>>>>>>>>>>> Test start [SSL_calcu_file_sha512] <<<<<<<<<<<<<<<<<\n");
	{
		unsigned char outSha[20];
		memset(outSha, 0, sizeof(outSha));
		SSL_calcu_file_sha512(filename, outSha, sizeof(outSha));
	}
	printf("Test end [SSL_calcu_file_sha512]\n\n");

	printf(">>>>>>>>>>>>>>> Test start [SSL_get_sha512_from_file] <<<<<<<<<<<<<<<<\n");
	{
		unsigned char outSha[20];
		memset(outSha, 0, sizeof(outSha));
		SSL_get_sha512_from_file(filename, outSha, sizeof(outSha));
	}
	printf("Test end [SSL_get_sha512_from_file]\n\n");

	printf(">>>>>>>>>>>>>>> Test start [SSL_recovery_sha512_file]<<<<<<<<<<<<<<<<<<\n");
	{
		char outSha[20];
		memset(outSha, 0, sizeof(outSha));
		SSL_recovery_sha512_file(filename, outSha, sizeof(outSha));
	}
	printf("Test end [SSL_recovery_sha512_file]\n");
	return 0;
}

int main(int argc, char** argv)
{
    SHA512_CTX stx;
    unsigned char outmd[20];//注意这里的字符个数为20
    unsigned char buffer[1024];
    char filename[512];
    int len=0;
    int i;
    FILE * fp=NULL;
    memset(outmd,0,sizeof(outmd));
    memset(filename,0,sizeof(filename));
    memset(buffer,0,sizeof(buffer));

	if (argc==2) {
		strcpy(filename, argv[1]);
	}
	else {
		printf("请输入文件名，用于计算SHA512值:");
		scanf("%s",filename);
	}
    fp=fopen(filename,"rb+");
    if(fp==NULL)
    {
        printf("Can't open file\n");
        return 0;
    }

    SHA512_Init(&stx);
    while((len=fread(buffer,1,1024,fp))>0)
    {
        SHA512_Update(&stx,buffer,len);
        memset(buffer,0,sizeof(buffer));
    }
    SHA512_Final(outmd,&stx);
	printf_buff(outmd, 20);
	fclose(fp);
	{
		unsigned char outSha[20];
		memset(outSha, 0, sizeof(outSha));
		SSL_calcu_file_sha512(argv[1], outSha, sizeof(outSha));
	}
	{
		printf(">>>>>>>>>>>>\n\n");
		SSL_Hash_file_test(filename);
		printf("\n\n\>>>>>>>>>>>>\n");
	}
    return 0;
}
