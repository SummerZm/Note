/*************************************************************************
 > File Name: convert-bst-to-greater-tree.cpp
 > Author: Leafxu
 > Created Time: 2020年07月30日 星期四 18时37分01秒
 *************************************************************************
/*
 *  问题：统计同值子树
 *  示例：
 *  输入：
 *  输出：   
 *  说明：同值子树是指该子树的所有节点都拥有相同的数值。
 *  要求：  
 *
 *  完成度：60%
 *  解法： 后序遍历 [子树问题一般使用后序遍历]
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/count-univalue-subtrees/
 */
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    int countUnivalSubtrees(TreeNode* root) {
        int res;
        postOrder(root, res);
        return res;
    }

    bool postOrder(TreeNode* root, int& res) {
        bool left, right;
        if (root==nullptr) return true;
        left = postOrder(root->left, res);
        right = postOrder(root->right, res);
        // 判读当前结点，是否符合 【取反比较】
        if ((root->left && root->left->val!=root->val) || (root->right && root->right->val!=root->val)) {
            return false;
        }
        // 判断左右子树是否符合
        if (left && right) {
            res += 1;
        }
        return false;
    } 
};