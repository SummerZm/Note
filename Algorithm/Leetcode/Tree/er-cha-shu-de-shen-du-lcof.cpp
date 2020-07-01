/*************************************************************************
 > File Name: er-cha-shu-de-shen-du-lcof
 > Author: Leafxu
 > Created Time: 2020年06月16 星期二 18时36分52秒
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
  *  问题：二叉树最深路径。
  *  示例：
  *  输入：
  *  输出：   
  *  要求：  
  *  说明：
  *
  *  完成度：60%
  *  解法：前序遍历， 后序遍历【尾递归加速】
  *  应用：
  *     
  *  分类：数据分析
  *  链接：https://leetcode-cn.com/problems/er-cha-shu-de-shen-du-lcof/
  */
class Solution {
public:
    int maxDepth(TreeNode* root) {
        int currDepth = 0;
        _maxDepth(root, &currDepth, 0);
        return currDepth;
    }
    
    /*
     * 标准思路三件套：终止，处理，遍历方式.
     */
    void _maxDepth(TreeNode* root, int* currDepth, int level) {
        if (root==NULL) return;
        if (++level > *currDepth) *currDepth=level; 
        _maxDepth(root->left, currDepth, level);
        _maxDepth(root->right, currDepth, level);
    }

    /*
     * 思路：高级，每递归一层就加1.
     * 说明：【后序遍历】从左右子树中选择最大的加1；尾递归加速。
     */
    int maxDepth1(TreeNode* root) {
        if(root == NULL) return 0;
        //return max(maxDepth1(root->left), maxDepth1(root->right)) + 1;
        return max(maxDepth1(root->left)+1, maxDepth1(root->right)+1);
    }
};