/*************************************************************************
 > File Name: lowest-common-ancestor-of-a-binary-tree.cpp
 > Author: Leafxu
 > Created Time: 2020年04月29日 星期三 12时36分52秒
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
  *  问题：给定一个二叉树, 找到该树中两个指定节点的最近公共祖先
  *  示例：
  *  输入：
  *  输出：   
  *  要求：  
  *  说明：
  *     1. 所有节点的值都是唯一的。
  *     2. p、q 为不同节点且均存在于给定的二叉树中。
  *
  *  完成度：
  *  解法：
  *     1. 树的后序遍历，处理完的左右子树.  [重点：树的不同遍历方法提供了处理节点的时机]
  *     
  *  分类：数据处理
  *  链接：https://leetcode-cn.com/problems/lowest-common-ancestor-of-a-binary-tree/
  */

class Solution {
public:
    /* 
        A. 递归代码结构整个有三层：
            1. 当前层要解决的问题.
            2. 如何进入下一层.
            3. 如何回到上一层.
        B. 要识别问题的难点在哪一层                   
    */
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // <1> 递归 如果p和q在一边, 我们返回那一边它们的最近祖先; 在两边, 那么最近祖先就是root;遇到了p或者q, 就返回p或q, 否则返回NULL
        // 【当前层】
        if (root == nullptr|| root == p || root == q ) {
            return root;
        } 
        // 【进入下一层】  
        TreeNode* left =  lowestCommonAncestor(root->left, p, q); //在左子树和右子树中分别去找p,q的最近公共祖先
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        // 【返回上一层】
        if(left && right) // p和q分别在root的左右两棵子树, 那么root就是最近公共祖先
            return root;
        else if(left == nullptr) // 说明p和q都不在左子树里, 那么只能返回右子树的结果
            return right;
        else if(right == nullptr)// 说明p和q都不在右子树里, 只能返回左子树的结果
            return left;      
        else return nullptr; // 这棵子树没有p或q 返回nullptr
    }
};
