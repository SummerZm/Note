/*************************************************************************
 > File Name: 3sum.cpp
 > Author: Leafxu
 > Created Time: 2020年09月16日 星期三 10时29分11秒
 ************************************************************************/
/*
 *  问题：给你一个包含n个整数的数组 nums，判断 nums 中是否存在三个元素 a，b，c ，使得 a + b + c = 0 ？请你找出所有满足条件且不重复的三元组。
 *  示例：
 *  输入：nums = [-1, 0, 1, 2, -1, -4]
 *  输出：[[-1, 0, 1],[-1, -1, 2]]
 *  要求：答案中不可以包含重复的三元组。
 *  说明：
 *
 *  完成度：30%
 *  解法：排序，双指针法/哈希法
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/3sum/s
 */
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> res;
        sort(nums.begin(),nums.end());
        // nums.erase(unique(nums.begin(), nums.end()), nums.end()); 不能在这里进去去重
        if(nums.size()<3||nums.front()>0||nums.back()<0) return {};
        
        for(int i=0;i<nums.size();i++)
        {
            int fix=nums[i];
            if(fix>0) break;   // 大于0, out
            if(i>0&&fix==nums[i-1]) // 去重
                continue;
            int l=i+1;
            int r=nums.size()-1;
            while(l<r)
            {
                if(nums[l]+nums[r]==-fix)
                {
                    /* 逻辑上带来的复杂度 */
                    if(l==i+1 || r==nums.size()-1)  // 刚好第一个就找到
                    {
                        res.push_back(vector<int>{nums[i],nums[l],nums[r]});
                        l++;
                        r--;
                    }
                    else if(nums[l]==nums[l-1]) // 去重
                        l++;
                    else
                    {
                        res.push_back(vector<int>{nums[i],nums[l],nums[r]});
                        l++;
                        r--;
                    }
                }
                else if(nums[l]+nums[r]<-fix)   // 双指针夹逼
                    l++;
                else r--;   // // 双指针夹逼
            }
        }
        return res;
    }
};

