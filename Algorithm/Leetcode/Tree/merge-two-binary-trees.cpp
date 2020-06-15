/*************************************************************************
 > File Name: merge-two-binary-trees
 > Author: Leafxu
 > Created Time: 2020年06月15 星期三 12时36分52秒
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
  *  问题：给定两个二叉树，将他们合并为一个新的二叉树。
  *  示例：
  *  输入：
  *  输出：   
  *  要求：  
  *  说明：合并的规则是如果两个节点重叠，那么将他们的值相加作为节点合并后的新值，否则不为 NULL 的节点将直接作为新二叉树的节点。
  *
  *  完成度：60%
  *  解法：树结构指针关系的维护和对结点的修改，前序遍历
  *  应用：Git记录历史树的合并
  *     
  *  分类：数据构造
  *  链接：https://leetcode-cn.com/problems/merge-two-binary-trees/
  */
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
        if (t1==NULL) return t2;
        if (t2==NULL) return t1;
        // 叶子结点的修改
        t1->val += t2->val;
        // 树指针关系的重新构造
        t1->left = mergeTrees(t1->left, t2->left);
        t1->right = mergeTrees(t1->right, t2->right);
        return t1;
    }
};