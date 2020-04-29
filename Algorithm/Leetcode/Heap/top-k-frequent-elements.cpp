/*************************************************************************
 > File Name: top-k-frequent-elements.cpp
 > Author: Leafxu
 > Created Time: 2020年04月28日 星期二 12时43分00秒
 ************************************************************************/

/*
  *  问题：给定一个非空的整数数组，返回其中出现频率前 k 高的元素
  *  示例：
  *  输入:   nums = [1,1,1,2,2,3], k = 2
  *  输出:   [1,2]
  *  要求:   算法的时间复杂度必须优于 O(n log n) , n 是数组的大小。可以按任意顺序返回答案。
  *
  *  完成度：20%
  *  解法：
  *  注解：纯粹用数据结构解决问题. 语言的熟悉度.
  *  理解：编程就是在不同场景下使用 判断，分支，循环对数据进行存储，分析，修改
  *                 1.  同步，异步， 并发， 时间/空间限制
  *  链接：https://leetcode-cn.com/problems/top-k-frequent-elements
  */

class Solution {
public:
	struct cmp
	{
		bool operator()(pair<int, int>& a, pair<int, int>& b){ return a.second > b.second; }
	};

	vector<int> topKFrequent(vector<int>& nums, int k) {
		vector<int> ret;
		/*
			一维-数据频率统计  O(NlogN)
											4 -> 2
											5 -> 1
			[4,4,5,6,6,6,7,8,8,8,8,]	=>	6 -> 3 
											7 -> 1
											8 -> 4 
			底层储存：Hash
		*/

		 // Hash表
		map<int, int> hash;
		for (auto a : nums)
		{
			hash[a]++;
		}

		// 优先队列 
		// priority_queue<数据类型, 数据储存的容器, 比较函数> 
		priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> freq;
		for (auto a : hash)
		{
			// 优先队列底层是用堆实现的
			// cmp: 决定堆的排序
			freq.push(a);
			if (freq.size() > k)
				freq.pop();
		}

		while (!freq.empty())
		{
			ret.push_back(freq.top().first);
			freq.pop();
		}
		return ret;
	}
};
