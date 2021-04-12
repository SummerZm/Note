### C语言泛型编程
	1. 增加了接口的复杂度
		void swap(void* x, void* y, size_t size)
		{
			 char tmp[size];
			 memcpy(tmp, y, size);
			 memcpy(y, x, size);
			 memcpy(x, tmp, size);
		}

	2. 用宏实现泛型的缺陷
		#define min(x, y)  （(x)>(y) ? (y) : (x)）
		当参数为 i++ 或 foo() 类似的输入时，会导致自加/调用2次


	3. 更复杂的C语言泛型例子
		int search(void* a, size_t size, void* target, 
			size_t elem_size, int(*cmpFn)(void*, void*) )
		{
			for(int i=0; i<size; i++) {
				// why not use memcmp()
				// use unsigned char * to calculate the address
				if ( cmpFn ((unsigned char *)a + elem_size * i, target) == 0 ) {
					return i;
				}
			}
			return -1;
		}

		传入的比较函数(勉强能用)：
		int int_cmp(int* x, int* y)
		{
			return *x - *y;
		}

		int string_cmp(char* x, char* y){
			return strcmp(x, y);
		}
	
		但当上面函数用于比较复杂的数据结构类型时就很复杂了

	4. C语言的问题：
		-- 泛型算法需要适配数据类型
			a. 实现泛型时只能通过void*或宏但这两种都太宽松,很容易出错
			b. 识别不了被void*或宏泛型化的数据类型，只能引入size参数，使程序变复杂了

		-- 泛型算法需要适配数据结构
			a. 复杂的数据结构是由简单类型的通过struct堆叠起来的，数据结构成员的复制，内存分配，释放非常不一样增加了复杂度

	5. C语言特性总结
		-- 不会阻止程序员做任何底层的事
		-- 保持语言的最小和最简特性
		-- C语言的强大与优雅在于：既具有高级语言的特性，又能简单的做任何底层上的微观控制。
		-- C语言基于过程和底层的初衷设计方式就会成为它的短板, 因为在编程这个世界中，更多的编程工作是在解决业务上的问题，而不是计算机的问题.


### C++的泛型编程
	1. 通过C语言的泛型可以得出泛型编程的需求：
		-- 算法的泛型
		-- 类型的泛型  【用模板数技术来抽象类型】
		-- 数据结构的泛型	
			a. C语言中的search(...)函数里面使用的是顺序的遍历方法仅适用于顺序型数据结构，对于图/树等非线性数据结构就明显不顶用来
			b. 需要找到一种泛型的数据结构操作方式 【使用迭代器来遍历/操作数据结构内的元素】

	2. 解决以上需求:
		-- 通过类的方式: 自定义类型和内建数据类型的一致(比较/分配/释放/等)
		-- 通过模板达到类型和算法的妥协
			a. 用模板数技术来抽象类型 - 数据容器
			b. 使用迭代器来遍历/操作数据结构内的元素 
			c. 解决C语言宏中常见的问题

		-- 通过过多态和运行时类型识别
			a. 支持同一类型的类型泛型
			b. 运行支持对具体类型的特殊处理

		// 泛型台阶1
		template<typename T, typename Iter>
		Iter search(Iter pStart, Iter pEnd, T target) 
		{
			for(Iter p = pStart; p != pEnd; p++) {
				if ( *p == target ) 
					return p;
			}
			return NULL;
		}
	
		// 泛型台阶2
		template<typename T, typename Iter>
		T sum(Iter pStart, Iter pEnd) {
			T result = 0;	// 这里的0是泛型编程中不应该出现的，因为0默认了类型
			for(Iter p=pStart; p!=pEnd; p++) {
				result += *p;
			}
			return result;	
		}

		// 改进-<实现自定义类型的迭代器-传参进来>
		template <class Iter>
		typename Iter::value_type
		sum(Iter start, Iter end, T init) {
			typename Iter::value_type result = init;
			while (start != end) {
				result = result + *start;
				start++;
			}
			return result;
		}

		// 泛型台阶3 -- 实现算法业务的泛型
		struct Employee {
			string name;
			string id;
			int vacation;
			double salary；
		};

		template<class Iter, class T, class Op>
		T reduce (Iter start, Iter end, T init, Op op) {
			T result = init;
			while ( start != end ) {
				result = op( result, *start );
				start++;
			}
			return result;
		}

		// 使用 -- 泛型回调
		double sum_salaries = 
  			reduce( staff.begin(), staff.end(), 0.0,{return s + e.salary;}  );

		double max_salary = 
			reduce( staff.begin(), staff.end(), 0.0, {return s > e.salary? s: e.salary; } );

		// 函数对象 - 封装更复杂的算法
		template<class T, class Cond>
		struct counter {
			size_t operator()(size_t c, T t) const {
				return c + (Cond(t) ? 1 : 0);
			}
		};

		template<class Iter, class Cond>
		size_t count_if(Iter begin, Iter end, Cond c){
			return reduce(begin, end, 0, counter<Iter::value_type, Cond>(c));
		}
	
		// 计算员工工资大于10000的个数 
		// 将条件抽出来	- 至于是什么条件属于业务逻辑，交由使用方决定
		// 如何从需求分析出真正的业务逻辑（ex：这里的业务逻辑不是个数而是条件）
		size_t cnt = count_if(staff.begin(), staff.end(), 
					{ return e.salary > 10000; });

### 类型系统和泛型编程的本质
	1. 从另一个角度看待编程需要处理好的问题有哪些
		a. 编程语言中的类型问题
		b. 对真实世界中业务代码的抽象，重用和拼装

	2. 类型系统的作用 
		a. 如何将编程语言中的数值和表达式归类，将它们进行操作，它们之间如何互相作用
		b. 编程语言类型系统起源于计算机架构实际问题,编译器实现，以及语言设计
		c. 功能：
			-- 程序语言的安全性  [编译时的类型检查]
			-- 有利于编译器的优化 [字节对齐]
			-- 代码可读性	[语义更清晰]
			-- 抽象化 [抽象数据在硬件内存中的细节]

		d. 问题：同一个算法需要根据不同类型实现相似的代码
		
	3. 类型检查系统
		a. 无论哪种语言都避免不了一个特定的类型系统, 所以任何编程语言都有类型检查系统
			-- 不同语言的类型系统的产生的限制程度不同，限制越强，更高层次抽象代码的书写的复杂越高

		b. 静态类型检查系统
			-- 一般在编译时检查 [强类型:自动类型转换时不允许信息丢失;反之为弱类型]
			-- 早期发现错误，提高运行时性能

		c. 动态类型检查系统
			-- 一般在运行时检查 [这类语言一般有is_array(), is_int(), ..]
			-- 更容易关注业务，但需要运行时的类型检查识别代码，带来效率问题

		d. 类型的本质
			-- 类型是对内存的一种抽象
			-- 不同类型有不同操作，所以，对特定类型有特定的一组操作	
			-- 类型系统的出现主要是对容许混乱的操作加上了严格的限制，以避免代码以无效的数据使用方式编译或运行。
			-- 类型的产生和限制，虽然对底层代码来说是安全的，但是对于更高层次的抽象产生了些负面因素。
				比如在 C++ 语言里，为了同时满足静态类型和抽象，就导致了模板技术的出现，带来了语言的复杂性。


	4. 泛型的本质
		a. 要了解泛型的本质，需要先理解类型的本质
		b. 本质：
			-- 屏蔽数据和操作数据的细节
			-- 专注于算法的结构，而不是算法中处理不同的数据类型

		c. 编程语言本质上帮助程序员屏蔽底层机器代码的实现，而让我们可以更为关注于业务逻辑代码。
		d. 编程语言作为机器代码和业务逻辑的粘合层。让程序员可以控制更多底层的灵活性还是屏蔽底层关注于业务逻辑，是件难两全的事。
	






































