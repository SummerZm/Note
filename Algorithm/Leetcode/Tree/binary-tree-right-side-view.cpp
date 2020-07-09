/*************************************************************************
 > File Name: binary-tree-right-side-view.cpp
 > Author: Leafxu
 > Created Time: 2020年07月09日 星期四 19时42分12秒
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
 *  问题：给定一棵二叉树，想象自己站在它的右侧，按照从顶部到底部的顺序，返回从右侧所能看到的节点值。
 *  示例：
 *  输入：
 *  输出：
 *  要求：
 *  说明：
 *
 *  完成度：60%
 *  解法：前序遍历
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/binary-tree-right-side-view/
 */
 /* 如何判断当前结点是：最右结点*/
class Solution {
    vector<int> res;
    void dfs(TreeNode* root, int level) {
        if (root == nullptr) return; 
        /* 巧妙的利用当前容器数组长度记录曾经到达的深度 */
        if (level>res.size()) {
            res.push_back(root->val);
        }
        dfs(root->right, level+1);
        dfs(root->left, level+1);
        return;
    }

public:
    vector<int> rightSideView(TreeNode* root) {
        dfs(root, 1);
        return res;
    }
};
