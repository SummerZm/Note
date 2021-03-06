## **集合，逻辑，哥德尔不完备定理**

### **皮亚诺定理**
> **用集合与逻辑定义自然数**  

- 定义1: 1是自然数
- 定义2: n是自然数，则n'也是自然数 【如何定义自然数的增长】
- 定义3: 对于自然数n, n'!=1      【如何防止自然数集出现重复】
- 定义4: 对于自然数n,m, 若n'=m',则n=m   【如何防止自然数集中出现汇合，循环的情况】
- 定义5: 命题P(1)成立，且当P(k)成立时，P(k‘)成立，则对于任意自然数n, P(n)成立。 【数学归纳法】
    1. 提供了如何证明：“对于任何自然数，P(x)成立“ 的方法
    2. 数学归纳法出现在了自然数的证明中，侧面反映了自然数本质与数学归纳法的紧密联系
    
- 如何定义自然数的加乘法
> **在上面自然数的定义中，我们一直没有使用+，额是使用‘表示自然数的后继数**

```sh
# 自然数加法运算的公理
# 1. 自然数n, n+1=n';
# 2. 自然数n,m, 有 n+m' = (n+m)' 【不可缺少】

# 自然数乘法运算的公理
# 1. 自然数n, n*1 = n
# 2. 自然数n,m; n*m' = n*m+n
# 

# 上面公理的定义的要点在于：自然数1 和 自然数后续‘ 的考虑
# 上面的公理定义都是集合语言的中译版本。从另外的角度看，可以用集合定义数学运算符号
```
### **形式系统表示数学**
>**莱布尼兹之梦**
- 对应关系
数学 ----》 形式系统
命题 ----》 逻辑公式
公理 ----》 形式公理
证明 ----》 形式证明
定理 ----》 形式定理
推理规则MP

### **无限与极限**
> **从集合的角度看待无限**  
- **整体与局部之间存在双射的集合称为无限集合**
- **满射，单射，双射的概念**

> **数学的探索进程: 要向新的道路前进时，任谁都会犹豫啊**  
-  不存在+1等于0的自然数，于是有了负数-1的定义
-  不存在平方后等于2的有理数，于是有了根号2的定义
-  不存在平方后等于-1的实数，于是有了虚数单位i的定义
-  在整体与部分之间存在双射，那就将当成无限集合的定义

> **从数列和函数的角度看待极限**
- 存在处处不联系的函数。
- 存在只在一点处连续的函数。【在实数范围内】
- 存在在有理数不连续，而在无理数连续的函数。

## **命题逻辑与形式系统**
> **用数学研究数学**


## **对角论证法**
> **实数编号**
- 定义: 可数集-能用自然数给集合元素编号的集合
- 证明：实数不是可数集？
- 证明：由0 < x < 1 所有实数构成的集合为不可数集？
- 知识：自然数不存在无穷大这个数
- 注意：对角论证法不能用于证明有理数集是不是可数集

## **形式系统的形式系统(算术与形式系统互相转化)**
> **相容的形式系统：对于形式系统的任意语句A，A和非A无法同时成立**

- 哥德尔第一完备定理：满足某一条件的形式系统，存在同时满足以下两个条件的语句A
	1. 该形式系统中不存在A的形式证明
	2. 该形式系统中不存在非A的形式证明

- 名词解释
	1. 哥德尔数
	2. 逻辑公式测定仪
	3. 公理测定仪
	4. 证明测定仪
	5. 形式系统的形式系统: 用形式系统表示算术；用算术表示形式系统

- 词汇整理  

含义的世界  ====> 形式的世界
算术		====> 算术的形式系统
谓词和命题  ====> 逻辑公式
谓词		====> 包含自由变量的逻辑公式
命题        ====> 不包含自由变量的逻辑公式（语句）
自然数      ====> 数项
	
### **等价关系**
> **集合除以等价关系得到集合。称为商集**

- **等价关系的三大性质：对称律，反身律， 传递律. [加深了等价关系的理解]**
- **‘对’的算术推演思考的过程值得学习，并有助于打开思路得到更严谨习惯** 
	1. ‘对’，向量，射影 ==》 向量和的影子

### **希尔伯特计划 & 哥德尔不完备定理**

- **希尔伯特计划**
	1. 导入形式系统来表示数学
		```sh
		# 把数学当成形式系统
		# 把形式系统中的逻辑公式定义为符号的序列，并从定义了公理的概念
		# 定义了推理规则；源于公理通过推理规则得到的逻辑公式被称为定理；这个过程中涉及的逻辑公式的序列被称为形式证明
		```
	2. 证明相容性：证明用于表述数学的形式系统不存在矛盾
		```sh
		# 希尔伯特使用排除了含义的形式系统来明确证明该形式系统系自身不存在矛盾
		```
	3. 证明完备性：证明用于表示数学的形式系统是完备的
		```sh
		# 相容性：对于形式系统的语句A，A和非A不能同时被证明
		# 完备性：对于形似系统的语句A，A和非A至少有一个成立
		```
- **对于完备形式系统的理解**
	1. 对完备的形式系统来说，哪怕往公理里加一个无法从形式上证明的语句，都会出现矛盾
	

- **哥德尔不完备定理**
> **不完备不是失败或是缺点，有可能是通往新世界的入口**
	1. 定理1；在满足某个条件的形式系统中存在语句A使得：既存在A的证明，也存在非A的证明
	2. 定理2: 在满足某个条件的形式系统中，不存在表示“自身系统相容性的的语句A”的形式证明
	3. 如果想证明某个系统的相容想，就要使用更强的系统

- **形式系统P的定义**
 	1. 证明概要
	2. 定义常量
	3. 定义变量
	4. 定义数项
	5. 定义符号
	6. 定义逻辑公式
	7. 定义省略形式
	8. 把皮亚诺公理导入形式系统p中
	9. 把命题逻辑的公理导入形式系统p中
	10. 把谓词逻辑的公理导入形式系统p中
	11. 将集合的内涵公理导入形式系统p中
	12. 将集合的外延公理导入形式系统p中
	13. 将推理规则导入形式系统p中
	
- **哥德尔数**
> **哥德尔数把形式证明译成了编码: 一个形式系统能够理解数，那么这个形式系统就能理解形式证明**
	1. 定义：基本符号的哥德尔数
	2. 定义：序列的哥德尔数
	3. 哥德尔数质因数分解时出现2的个数：2个数是奇树=>符号序列； 如果是偶数=>符号序列的序列 【未看懂】
 
- **原始递归函数**
	1. 原始递归定义
	2. 原始递归函数
	3. 原始递归谓词
	4. 表现定理：
		【因为形式系统P能描述数论，所以表现定理成立。主要是因为形式系统P中的逻辑公式都能用哥德尔数表示】
		【形式的世界可以判定命题在含义的世界是否成立】
		【把数代进谓词可得到的命题是否成立，能够通过形式证明来判定】
		【当不存在原始递归性时，在形式系统P中就不一定存在该谓词的逻辑公式.若不存在形式公式，则谈不上形式证明】

	5. 谓词和命题：
	6. 逻辑公式和语句：

- **证明测定仪：用原始递归构建 'p是x' 的形式证明**
> **一共推出46个定义, 前45个定义都具有原始递归性，第46定义不是原始递归的**

_ **形式世界和含义的世界**
	含义的世界     				     形式的世界
	算术递归谓词  ----表现定理--->	 逻辑公式
	    数        <---哥德尔数----   符号/逻辑公式/形式证明

- **不可判断语句**
> 在形式系统中无法在形式上证明的语句。即：不可判断语句


哥德尔没有将逻辑公式作为逻辑公式来处理，而是使用哥德尔数来表示逻辑公式，然后用数（在含义的世界里）生成关于形式系统的谓词，把元数学的观点展开里了
这样成功的论述了自身，也就是成功的自我指涉


































	










