/*************************************************************************
 > File Name: er-cha-shu-de-jing-xiang-lcof
 > Author: Leafxu
 > Created Time: 2020年06月12日 星期三 12时40分52秒
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
  *  问题：二叉树的镜像
  *  示例：
  *  输入：
  *  输出：   
  *  要求：  
  *  说明：
  *
  *  完成度：80%
  *  解法：前序后序遍历都可以解决这个问题【需要遍历整棵树】
  *  应用： 
  *     
  *  分类：数据构造
  *  链接：https://leetcode-cn.com/problems/er-cha-shu-de-jing-xiang-lcof/submissions/
  */
class Solution {
public:
    TreeNode* mirrorTree(TreeNode* root) {
        if (root == NULL) {
            return NULL;
        }
        mirrorTree(root->left);
        mirrorTree(root->right);
        swap(root->left, root->right);
        return root;
    }
};