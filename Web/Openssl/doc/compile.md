## Openssl uclibc 交叉编译##

./config 
    no-asm no-shared
    --cross-compile-prefix=/usr/local/arm/arm-hisiv300-linux/target/bin/arm-hisiv300-linux-
    -march=armv7-a
    -mfloat-abi=softfp
    -mfpu=neon