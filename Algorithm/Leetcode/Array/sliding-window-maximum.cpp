/*************************************************************************
 > File Name: reverseGroup.cpp
 > Author: Leafxu
 > Created Time: 2021年04月01日 星期四 20时49分11秒
 ************************************************************************/
/*
 *  问题：
 * 给你一个整数数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧。你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。
 * 返回滑动窗口中的最大值。
 * 
  示例：
    输入：nums = [1,3,-1,-3,5,3,6,7], k = 3
    输出：[3,3,5,5,6,7]
    解释：
    滑动窗口的位置                最大值
    ---------------               -----
    [1  3  -1] -3  5  3  6  7       3
    1 [3  -1  -3] 5  3  6  7       3
    1  3 [-1  -3  5] 3  6  7       5
    1  3  -1 [-3  5  3] 6  7       5
    1  3  -1  -3 [5  3  6] 7       6
    1  3  -1  -3  5 [3  6  7]      7

 *  说明：
 *
 *  完成度：90%
 *  解法：
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/sliding-window-maximum/
 */
/*
*  1. 对现有数据结构进行二次封装，定制
*  2. 封装细节。用人类的思想语言习惯描述程序逻辑
*  3. 单调队列在添加元素的时候靠删除元素保持队列的单调性，相当于抽取出某个函数中单调递增（或递减）的部分；而优先级队列（二叉堆）相当于自动排序，差别大了去了。
* 
* */
class Solution {
    // 对现有数据结构进行二次封装，定制
    class MonotonicQueue
    {
        private:
            deque<int> data;
        public:
            void push(int n)
            {
                while (!data.empty() && data.back()<n)
                    data.pop_back();
                data.push_back(n);
            }
    
            int max() { return data.front();}

            void pop(int n)
            {
                if (!date.empty() && data.front()==n)
                    data.pop_front();
            }
    };

public:
    // 用人类的思想语言习惯描述程序逻辑
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        MonotonicQueue window;
        vector<int> res;
        for (int i=0; i<nums.size(); i++)
        {
            if (i< k-1)
            {
                window.push(nums[i]);
            }
            else 
            {
                window.push(nums[i]);
                res.push_back(window.max());
                window.pop(nums[i-k+1]);
            }
        }
        return res;
    }
};