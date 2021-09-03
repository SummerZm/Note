1. linux 下编译：
    A. ss_env linux
    B. gcc -o sha1 sha1.c -I../../lib/uclibc/openssl/include -L../../lib/uclibc/openssl/lib -lssl -lcrypto

2. uclibc 下编译：
    A. ss_env linux
    B. arm-hisiv300-linux-gcc -o sha1 sha1.c -I../../lib/uclibc/openssl/include -L../../lib/uclibc/openssl/lib -lssl -lcrypto
