/*************************************************************************
 > File Name: binary-search-tree-K-Node.cpp
 > Author: Leafxu
 > Created Time: 2020年06月23日 星期二 19时32分12秒
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
 *  问题：二叉搜索树的第k大节点。
 *  示例：
 *  输入：
 *  输出：
 *  要求：
 *  说明：
 *
 *  完成度：70%
 *  解法：中序遍历
 *  注解：如何在遍历的过程中把想要的数据保存下来？
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/er-cha-sou-suo-shu-de-di-kda-jie-dian-lcof/
 */
class Solution {
public:
    int kthLargest(TreeNode* root, int k) {
        /* 简单递归参数只在本层可见，所以必须引入n,res栈变量.*/
        int res = 0, n = 0;
        dfs(root, k, &n, &res);
        return res;
    }

    void dfs(TreeNode* root, int k, int* n, int* res){
        if (root==NULL) return;
        dfs(root->right, k, n, res);
        // k-- 不能使用
        if (k==++(*n)) *res = root->val; /* 优先级问题 */
        dfs(root->left, k, n, res);
        return;
    }
};