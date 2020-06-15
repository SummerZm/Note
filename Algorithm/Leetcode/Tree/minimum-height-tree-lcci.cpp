/*************************************************************************
 > File Name: minimum-height-tree-lcci
 > Author: Leafxu
 > Created Time: 2020年06月11日 星期三 12时36分52秒
 ************************************************************************/
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

/*
  *  问题：给定一个有序整数数组，元素各不相同且按升序排列，编写一个算法，创建一棵高度最小的二叉搜索树。
  *  示例：
  *  输入：
  *  输出：   
  *  要求：  
  *  说明：
  *
  *  完成度：80%
  *  解法：
  *  应用： 如何从数组中构建一个二叉树？[先排序形成中序排列，之后直接从中间排列生成平衡二叉树.]
  *     
  *  分类：数据构造
  *  链接：https://leetcode-cn.com/problems/minimum-height-tree-lcci/
  */

class Solution {
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return dfs(nums, 0, nums.size()-1);
    }

    TreeNode* dfs(const vector<int> &nums, int L, int R) {
        if(L > R) {
            return nullptr;
        }
        int mid = (L+R)>>1;
        auto ptr = new TreeNode(nums[mid]); //填充根节点
        ptr->left = dfs(nums, L, mid-1); //构造左子树
        ptr->right = dfs(nums, mid+1, R); //构造右子树
        return ptr;
    }
};