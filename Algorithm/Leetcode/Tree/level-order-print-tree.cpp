/*************************************************************************
 > File Name: level-order-print-tree.cpp
 > Author: Leafxu
 > Created Time: 2020年07月10日 星期五 13时12分12秒
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
  *  问题：“之”形层序遍历二叉树
  *  示例：
  *  输入：
  *  输出：
  *  要求：
  *  说明：
  *
  *  完成度：60%
  *  解法： 层序遍历扫描的方向和左右结点加入的方向
  *  应用：
  *     
  *  分类：数据分析
  *  链接：https://leetcode-cn.com/problems/cong-shang-dao-xia-da-yin-er-cha-shu-iii-lcof/
  */
class Solution {
    void _levelOrder(vector<vector<int>>& res, deque<TreeNode*> que, int level) {

        int size = que.size();
        vector<int> currlevelvalset;
        if (size==0) return;

        while (size>0) {
            TreeNode* node = NULL;
            if (level%2==1) {
                node = que.front();
                que.pop_front();
                /* Scan : from right to left. */
                if (node->right) que.push_back(node->right);
                if (node->left) que.push_back(node->left);
            }
            else {
                node = que.back();
                que.pop_back();
                /* Scan : from left to right. */
                if (node->left) que.push_front(node->left);
                if (node->right) que.push_front(node->right);
            }
            size--;
            currlevelvalset.push_back(node->val);
        }
        res.push_back(currlevelvalset);
        _levelOrder(res, que, level+1); /* if level++ then error. */
    }

public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        deque<TreeNode*> que;
        if (root!=nullptr) {
            que1.push_front(root);
            _levelOrder(res, que, 0);
        }
        return res;
    }
};