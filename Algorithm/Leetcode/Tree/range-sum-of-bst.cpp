/*************************************************************************
 > File Name: range-sum-of-bst.cpp
 > Author: Leafxu
 > Created Time: 2020年06月15日 星期三 12时40分52秒
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
  *  问题：一定数值范围之内二叉树结点之和
  *  示例：
  *  输入：
  *  输出：   
  *  要求：  
  *  说明：
  *
  *  完成度：60%
  *  解法：前序遍历 【后序遍历需要处理整颗二叉树不建议】， 中序遍历, O(n)
  *  应用： 
  *     
  *  分类：数据构造
  *  链接：https://leetcode-cn.com/problems/range-sum-of-bst/
  */
class Solution {
public:
    /*
     * 思路：要去哪颗子树查找目标
     * 参照系：四个区间
     * 1. [L, curr]
     * 2. [curr,R]
     * 3. [min, L)
     * 4. (R, max]
     */
    int rangeSumBST(TreeNode* root, int L, int R) {
        static int result = 0;
        if (root==NULL) {
            return result;
        }

        if (root->val>=L && root->val<=R) {
            result += root->val;
            if (root->left) rangeSumBST(root->left, L, R);
            if (root->right) rangeSumBST(root->right, L, R);
        }
        else if (root->val<L && root->right) {
            rangeSumBST(root->right, L, R);
        }
        else if (root->val>R && root->left) {
            rangeSumBST(root->left, L, R);
        }
        return result;
    }

    // 解法二
    int ans;
    int rangeSumBST1(TreeNode* root, int L, int R) {
        ans = 0;
        dfs(root, L, R);
        return ans;
    }
    /*
     * 思路：要去哪颗子树查找目标
     * 参照系：两个区间
     * 1. [min, curr->val)
     * 2. (curr->val, max]
     */
    void dfs(TreeNode* node, int L, int R) {
        if (node==NULL)  ruturn;
        // 处理当前层
        if (L <= node->val && node->val <= R)
            ans += node->val;
        if (L < node->val)
            dfs(node->left, L, R);
        if (node->val < R)
            dfs(node->right, L, R);
    }

    // 解法三： 中序遍历
    int rangeSumBST2(TreeNode* root, int L, int R) {
        int sum = 0;
        DFS(root, sum, L, R);
        return sum;       
    }
    void DFS(TreeNode* root, int& sum,const int L, const int R){
        if(root==nullptr) return;
        //左子树
        if(root->left) DFS(root->left,sum,L,R);
        //如果当前结点的值在L-R之间，就相加
        if(root->val>=L && root->val<=R)sum += root->val;
        if (root->val>R) return;
        //右子树
        if(root->right) DFS(root->right,sum,L,R);      
    }
};