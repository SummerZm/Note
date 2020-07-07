/*************************************************************************
 > File Name: cousins-in-binary-tree.cpp
 > Author: Leafxu
 > Created Time: 2020年07月06日 星期一 13时23分17秒
 *************************************************************************
/*
 *  问题：判断二叉树中的两个数据是否为堂兄弟结点
 *  示例：
 *  输入：
 *  输出：   
 *  说明：只有与值 x 和 y 对应的节点是堂兄弟节点时，才返回 true。否则，返回 false。
 *  要求：如果二叉树的两个节点深度相同，但父节点不同，则它们是一对堂兄弟节点。  
 *
 *  完成度：40%
 *  解法：
 *  注解：
 *  分类：数据分析
 *  链接：https://leetcode-cn.com/problems/cousins-in-binary-tree/
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
    int x_h;
    int y_h;
    bool _isCousins;

    void dfs(TreeNode* root, int x, int y, int level) {
        if (root==nullptr) return;
        // 不能在这里给 x_h,y_h赋值，遍历方式混用往往是错的.
        dfs(root->left, x, y, level+1);
        dfs(root->right, x, y, level+1);

        if (root->val==x) {
            x_h = level;
        }
        else if (root->val==y) {
            y_h = level;
        }
        
        // else {  # error: 会导致忽略当前结点
        {
            if (x_h!=0 && y_h!=0) {
                if (x_h!=y_h) {    // 不在同一层
                    _isCousins = false;
                }
                else if ( (root->left && root->right) // 有相同父节点
                    && (root->left && (root->left->val==x || root->left->val==y))
                    && (root->right && (root->right->val==x || root->right->val==y))
                ) {
                    _isCousins = false;
                }
            }
            else if (level==0 && (x_h==0 || y_h==0)) { // 找到一个结点
                _isCousins = false;        
            }
        } 
        return;
    }

public:
    bool isCousins(TreeNode* root, int x, int y) { 
        x_h=0, y_h=0;
        _isCousins = true;
        dfs(root, x, y, 0);
        return _isCousins;
    }
};