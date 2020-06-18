/*************************************************************************
 > File Name: trim-a-binary-search-tree
 > Author: Leafxu
 > Created Time: 2020年06月18日 星期四 12时40分52秒
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
  *  问题：修剪二叉搜索树
  *  示例：
  *  输入：
  *  输出：   
  *  要求：给定一个二叉搜索树，同时给定最小边界L 和最大边界 R。通过修剪二叉搜索树，使得所有节点的值在[L, R]中 (R>=L) 
  *  说明：可能需要改变树的根节点，所以结果应当返回修剪好的二叉搜索树的新的根节点。
  *  完成度：60%
  *  解法：后序遍历 [修改构造原树]
  *  应用： 
  *     
  *  分类：数据构造 
  *     1. 如果在栈上分配内存，则不会出现内存泄漏情况]
  *     2. 函数的调用嵌套关系，是一层调用一层的，最后肯定会回到最上一层，只要我们在最上面一层分配栈内存，那这些内存就能够被释放到
  *     3. 其他情况：内存系统资源释放，
  *     
  *  链接：https://leetcode-cn.com/problems/trim-a-binary-search-tree/
  */
class Solution {
public:
    /* 后序遍历 */
    TreeNode* trimBST(TreeNode* root, int L, int R) {
        if (root == null)
        return root;

        // 如何进入下一层
        if (root.val < L)
            return trimBST(root.right, L, R);
        if (root.val > R)
            return trimBST(root.left, L, R);

        // 根据已知条件处理当前层结点
        // 已知条件：修剪值在[L, R]之外的结点
        root.left = trimBST(root.left, L, R);
        root.right = trimBST(root.right, L, R);
        return root;
    }
};




















