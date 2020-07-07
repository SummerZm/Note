/*************************************************************************
 > File Name: convert-bst-to-greater-tree.cpp
 > Author: Leafxu
 > Created Time: 2020年07月07日 星期二 18时37分01秒
 *************************************************************************
/*
 *  问题：把二叉搜索树转换为累加树
 *  示例：
 *  输入：
 *  输出：   
 *  说明：使得每个节点的值是原来的节点值加上所有大于它的节点值之和。
 *  要求：  
 *
 *  完成度：60%
 *  解法： 后序遍历 【加上右子树结点的和】[后序遍历往往伴随着全局或引用参数]
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/convert-bst-to-greater-tree/
 */
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
    /* 后序遍历 [当前结点添加右子树结点之和][题意理解错误]*/
    int dfs1(TreeNode* root, int rSum) {
        if (root == nullptr) return 0;

        dfs(root->left, rSum);
        rSum = dfs(root->right, rSum);

        /* 处理当前结点
         *  1. 计算rSum.
         *  2. 更新当前结点的值.
         *  注意： 步骤1-2：不能颠倒.
         **/
        rSum += root->val;
        root->val = rSum;
        return rSum;
    }

    /* 反向中序遍历 */
    int dfs(TreeNode* root, int sum) {
        if (root == nullptr) return sum;
        sum = dfs(root->right, sum);
        /* 处理当前结点
         *  1. 计算rSum.
         *  2. 更新当前结点的值.
         *  注意： 步骤1-2：不能颠倒.
         **/
        sum += root->val;
        root->val = sum;
        sum = dfs(root->left, sum);
        return sum;
    }

public:
    TreeNode* convertBST(TreeNode* root) {
        dfs(root, 0);
        return root;
    }
};