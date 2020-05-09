/*
 * 问题：给定一个数组，将数组中的元素向右移动 k 个位置，其中 k 是非负数。
 * 完成度: 30%
 * 前置条件: 空间复杂度为 O(1) 的 原地 算法
 * 法1：暴力每次移动1个位置，移动k次.
 * 法2：额外数组 - 空间复杂度 O(n).
 * 法3：把数组当成一个环 - 会形成闭环.
 * 法4：将数组进行两次反转.
 * 注解：NULL
 * 链接：https://leetcode-cn.com/problems/rotate-array/
 */
class Solution {
public:
    void rotate1(vector<int>& nums, int k) {
        int count = 0;
        int len = nums.size();
        k = k % len;
        // 将已经处理了几个当做循环的终止条件
        for (int start=0; count<len; start++) {
            int prev = nums[start];
            int current = start;
            do {
                int next = (current+k) % len;
                int temp = nums[next];
                nums[next] = prev;
                prev = temp;
                current = next;
                count++;
            } while (start != current);
        }
    }


    void rotate(vector<int>& nums, int k) {
        int tmp = 0;
        int len = nums.size();

        // For input: [[1,2,3]  4
        if (k>len) k -= len;

        // Reverse  ALL
        for (int i=0; i<len/2; i++) {
            tmp = nums[len-i-1];
            nums[len-i-1] = nums[i];
            nums[i] = tmp;
        }

        // Reverse  [0, k)
        for (int i=0; i<k/2; i++) {
            tmp = nums[k-i-1];
            nums[k-i-1] = nums[i];
            nums[i] = tmp;
        }
        
        // Reverse  [k, len)
        for (int i=k; i<(len+k)/2; i++) {
            tmp = nums[len-i-1+k];
            nums[len-i-1+k] = nums[i];
            nums[i] = tmp;
        }
    }
};
