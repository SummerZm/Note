/*************************************************************************
 > File Name: 2Sum.cpp
 > Author: Leafxu
 > Created Time: 2021年03月22日 星期二 8时42分12秒
 ************************************************************************/
/*
 *  问题：给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出 和为目标值 的那 两个 整数，并返回它们的数组下标。
 *  你可以假设每种输入只会对应一个答案。但是，数组中同一个元素在答案里不能重复出现。你可以按任意顺序返回答案。
 *
 *  示例：
 *    输入：nums = [3,2,4], target = 6
 *    输出：[1,2]
 *
 *  要求：
 *  说明：只会存在一个有效答案
 *
 *  完成度：100%
 *  解法：
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/two-sum/
 */
class solution
{
    public:
        vector<int> twoSum(vector<int>& nums, int target)
        {
            unordered_map<int, int> hashtable;
            for (int i=0; i<nums.size(); i++)
            {
                auto it = hashtable.find(target-nums[i]);
                if (it != hashtable.end())
                {
                    return {it->second, i};
                }
                // 两个数都在一个数组里，一定会匹配上
                hashtable[nums[i]] = i;
            }
            return {};
        }  
};