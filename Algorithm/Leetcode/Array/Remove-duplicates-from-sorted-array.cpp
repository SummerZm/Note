/*
 * 问题：给定一个排序数组，你需要在原地删除重复出现的元素，使得每个元素只出现一次，返回移除后数组的新长度。
 * 完成度: 30%
 * 前置条件: daat is order.
 * 注解: 第一次出现往前挪并交换，再次出现不处理.
 * 链接：https://leetcode-cn.com/problems/Remove-duplicates-from-sorted-array/ 
 */
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int n = 0;
        int len = nums.size();
        if (len==0) return 0;
        /* 快慢指针初始化有前后关系. - 不是都在0这个位置起步. */
        for (int i=1; i<len; i++) {
            /* 去重，出现不一样时，慢指针往前走一步，然后覆盖.*/
            if (nums[i]!=nums[n]) {
                n++;    /*  */
                nums[n] = nums[i];
            }
        }
        return n+1; /* 返回新数组的长度，用于覆盖多余的数据. */
    }
};
