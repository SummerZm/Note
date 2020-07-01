/*************************************************************************
 > File Name: search-in-a-binary-search-tree
 > Author: Leafxu
 > Created Time: 2020年06月15日 星期一 12时40分52秒
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
  *  问题：二叉搜索树中的搜索
  *  示例：
  *  输入：
  *  输出：   
  *  要求：  
  *  说明：
  *
  *  完成度：60%
  *  解法：前序遍历 ， 中序遍历, O(n)
  *  应用： 
  *     
  *  分类：数据分析
  *  链接：https://leetcode-cn.com/problems/search-in-a-binary-search-tree/solution
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
    /*
     *  思路： 前序遍历，便于理解体现思考过程
     */
    TreeNode* searchBST1(TreeNode* root, int val) {
        // 定义当前层临时变量,用于储存结果
        TreeNode* target = NULL;
        if (root==NULL) return NULL;
        // 前序处理当前层
        if (root->val==val) return root;
        // 根据相关逻辑进入下一层级
        target = searchBST1(root->left, val);
        if (target==NULL) {
            searchBST(root->right, val);
        }
        // 将结果往上抛，此处无资源回收问题
        return target;
    }

    /*
     *  思路： 前序遍历优化
     */
    TreeNode* searchBST1(TreeNode* root, int val) {
        /*
            if (root==NULL || root->val==val) return root;
            return (root->val > val) ? searchBST1(root->left, val) : searchBST1(root->right, val);
        */
        TreeNode* target = NULL;
        if (root==NULL) return NULL;
        if (root->val==val) {
            target = root;
        }
        else if (root->val>val) {
           target = searchBST(root->left, val);
        }
        else {  //  root->val<val
            target = searchBST(root->right, val);
        }
        return target;
    }
};