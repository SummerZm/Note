/*************************************************************************
 > File Name: binary-tree-upside-down.cpp
 > Author: Leafxu
 > Created Time: 2020年07月22日 星期三 18时32分12秒
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
 *  问题：将二叉树上下翻转并将它变成一棵树， 原来的右节点将转换成左叶节点。返回新的根。
 *  示例：
 *  输入：
 *  输出：
 *  要求：
 *  说明：这棵二叉树所有的右节点要么是具有兄弟节点（拥有相同父节点的左节点）的叶节点，要么为空。
 *
 *  完成度：
 *  解法：非常规遍历树
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/binary-tree-upside-down/
 */
class Solution {
public:
    TreeNode* upsideDownBinaryTree(TreeNode* root) {
        if (!root || (!root->left && !root->right)) {
            return root;
        }
        TreeNode* pos = root;
        TreeNode* pre = root;
        while(pos->left) {  // 双指针找根结点.
            pre = pos;
            pos = pos->left;
        }
        pos->left = pre->right;
        pre->left = pre->right = NULL;  // 阻断结点[配合双指针找根结点]
        pos->right = upsideDownBinaryTree(root); // 再一次从顶到底.
        return pos;
    }

    TreeNode* upsideDownBinaryTree1(TreeNode* root) {
        if (root == NULL || root->left == NULL) return root;
        // 保存当前层的树指针指向.
        auto l = root->left;
        auto r = root->right;
        // 清空-树指针指向关系.
        root->left = NULL;
        root->right = NULL;
        // 递归-进入最底层.
        auto res = upsideDownBinaryTree(l);
        // 重建当前层树指针指向关系.
        l->left = r;
        l->right = root;
        return res;
    }
};