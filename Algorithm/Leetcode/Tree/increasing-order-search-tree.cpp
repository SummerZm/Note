/*************************************************************************
 > File Name: increasing-order-search-tree
 > Author: Leafxu
 > Created Time: 2020年06月17 星期三 18时36分52秒
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
 *  问题：按中序遍历 重新排列树，使树中最左边的结点现在是树的根，并且每个结点没有左子结点，只有一个右子结点。
 *  示例：
 *  输入：
 *  输出：   
 *  要求：  
 *  说明：
 *
 *  完成度：60%
 *  解法：中序遍历用于改变原树的结构。[延伸：中序遍历有多种实现方式]； 中序遍历，构造新树
 *  应用：
 *     
 *  分类：数据构造
 *  链接: https://leetcode-cn.com/problems/increasing-order-search-tree/
 * 
 *  [延伸：中序遍历其他实现方式]
 *  链接: https://leetcode-cn.com/problems/increasing-order-search-tree/solution/dui-lie-die-dai-by-keyway1984/
 */
class Solution {
public:
    /* 移动滑标 */
    TreeNode* curr;
    TreeNode* increasingBST(TreeNode* root) {
        /* 头结点 */
        TreeNode* ans = new TreeNode(0);
        curr = ans;
        inorderDealwith(root);
        return ans->right;
    }

    /*
     *   二叉树结构修改基本思路
     * 1. 一个二叉树Node有3个相关指针：左右结点指针，父节点指向自己指针
     * 2. 基本方法：选用遍历方法，处理当前层结点的左右指针
     * 3. 难点：调整父节点指向自己指针 【需要保存当前层结点在下一层处理】
     * 4. 如何将当前层node, 延伸到下一层处理？
     *    a. 全局/静态保存 
     *    b. 递归函数返回值 [反转单链表]
     */
    void inorderDealwith(TreeNode* root) {
        if (root==NULL) return NULL;
        inorderChange(root->left);
        /* 
         * 处理当前结点：
         * 1. 左节点：中序遍历都被处理完，设为NULL不会出错。
         * 2. 右节点：保存到全局中去，在下一层递归处理
         */
        root->left = NULL;  // 处理当前结点  
        curr->right = root; // 上一层的入侵者
        curr = root ;       // 更新入侵者
        inorderChange(root->right);
    }
};

