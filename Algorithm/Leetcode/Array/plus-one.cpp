/*************************************************************************
 > File Name: move-zeroes.cpp
 > Author: Leafxu
 > Created Time: 2021年03月25日 星期四 上午
 ************************************************************************/
/*
 *  问题：
 *    给定一个由 整数 组成的 非空 数组所表示的非负整数，在该数的基础上加一。
 *    最高位数字存放在数组的首位， 数组中每个元素只存储单个数字。
 *    你可以假设除了整数 0 之外，这个整数不会以零开头。
 *
 *  示例：
 *   输入：digits = [1,2,3]
 *   输出：[1,2,4]
 *   解释：输入数组表示数字 123。
 *
 *  完成度：60%
 *  解法：
 *  注解：
 *  分类：数据处理
 *  思路：
 *  
 *  链接：https://leetcode-cn.com/problems/plus-one/
 */
class solution
{
    public:
        vector<int> push_one(vector<int>& nums)
        {
            /* Format 1： */
            for (int i=nums.size(); i>=0; i--)
            {
                nums[i]++;
                nums[i] %= 10;
                if (nums[i]!=0)
                {
                    return nums;
                }
            } 
            nums.insert(nums.begin(), 1);
            return nums;

            /* Format 2:
            for (int i=nums.size(); i>=0; i--)
            {
                nums[i]++;
                if (nums[i]%10!=0)
                {
                    return nums;
                }
                else 
                {
                    nums[i] = 0;
                }
            }
            nums.insert(nums.begin(), 1);
            return nums;
            */
        }
};
