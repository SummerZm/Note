1. linux 下编译：
    A. ss_env linux
    B. gcc -o sha1 sha1.c -I../../lib/linux/openssl/include -L../../lib/linux/openssl/lib -lssl -lcrypto

2. uclibc 下编译：
    A. ss_env linux
    B. arm-hisiv300-linux-gcc -o sha1 sha1.c -I../../lib/uclibc/openssl/include -L../../lib/uclibc/openssl/lib -lssl -lcrypto

3. SHA摘要算法输出简介
	A. SHA-1 已经被破解
	B. SHA-2 即(SHA-512) 未被破解

4. 家族成员
	A. SHA家族的五个算法，分别是SHA-1、SHA-224、SHA-256、SHA-384，和SHA-512，由美国国家安全局（NSA）所设计，并由美国国家标准与技术研究院（NIST）发布；是美国的政府标准。
	B. 后四者有时并称为SHA-2。SHA-1在许多安全协定中广为使用，包括TLS和SSL、PGP、SSH、S/MIME和IPsec，曾被视为是MD5（更早之前被广为使用的杂凑函数）的后继者。
	C. SHA-1的安全性如今被密码学家严重质疑；虽然至今尚未出现对SHA-2有效的攻击，它的算法跟SHA-1基本上仍然相似；因此有些人开始发展其他替代的杂凑算法。
	D. SHA-0和SHA-1可将一个最大2的64次方位元的讯息，转换成一串160位元的讯息摘要；其设计原理相似于MIT教授Ronald L. Rivest所设计的密码学杂凑算法MD4和MD5。

5. SHA-0和1
	A. 最初载明的算法于1993年发布，称做安全杂凑标准（Secure Hash Standard），FIPS PUB 180。这个版本常被称为SHA-0。
	B. SHA-1和SHA-0的算法只在压缩函数的讯息转换部分差了一个位元的循环位移。
	C. 2005年8月17日，王小云和殷益群、于红波再度发表了对SHA-0破密的算法，可在2的39次方的计算复杂度内就找到碰撞。
	D. 殷益群在一次面谈中如此陈述：“大致上来说，我们找到了两个弱点：其一是前置处理不够复杂；其二是前20个循环中的某些数学运算会造成不可预期的安全性问题。

6. SHA-2
	A. NIST发布了三个额外的SHA变体，这三个函数都将讯息对应到更长的讯息摘要。以它们的摘要长度（以位元计算）加在原名后面来命名
		1. SHA-256，SHA-384和SHA-512。它们发布于2001年的FIPS PUB 180-2草稿中，随即通过审查和评论。包含SHA-1的FIPS PUB 180-2，于2002年以官方标准发布。

	B. SHA-256和SHA-512是很新的杂凑函数，前者以定义一个word为32位元，后者则定义一个word为64位元。它们分别使用了不同的偏移量，或用不同的常数，然而，实际上二者结构是相同的
		1. SHA-256和SHA-512只在循环执行的次数上有所差异。SHA-224以及SHA-384则是前述二种杂凑函数的截短版，利用不同的初始值做计算。
		2. 这些新的杂凑函数并没有接受像SHA-1一样的公众密码社群做详细的检验，所以它们的密码安全性还不被大家广泛的信任。

	C. SHA1生成20Bytes = 160 bit，SHA256生成32Bytes = 256 bit， SHA512生成64Bytes = 512 bit .







































































