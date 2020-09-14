/*************************************************************************
 > File Name: move-zeroes.cpp
 > Author: Leafxu
 > Created Time: 2020年09月14日 星期一 10时26分19秒
 ************************************************************************/
/*
 *  问题：给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，同时保持非零元素的相对顺序。
 *  示例：
 *  输入: [0,1,0,3,12]
 *  输出: [1,3,12,0,0]
 *  要求：1. 必须在原数组上操作，不能拷贝额外的数组。2. 尽量减少操作次数。
 *  说明：
 *
 *  完成度：60%
 *  解法：[快慢指针-处理有序数据]
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/move-zeroes/
 */
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        for (int head=0, tail=0; tail<nums.size(); tail++) {
            if (nums[tail]!=0) {
                swap(nums[head], nums[tail]);
                head++;
            }
        }
    }
};

