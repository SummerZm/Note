/*************************************************************************
 > File Name: binary-tree-paths.cpp
 > Author: Leafxu
 > Created Time: 2020年07月02 星期四 12时36分52秒
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
/**
 *  问题：给定一个二叉树，返回所有从根节点到叶子节点的路径。
 *  示例：
 *  输入：
 *  输出：["1->2->5", "1->3"]
 *  要求：  
 *  说明：
 *
 *  完成度：60%
 *  解法：
 *  应用：
 *     
 *  分类：数据分析
 *  链接：https://leetcode-cn.com/problems/binary-tree-paths/
 */
class Solution {
    /* 1. path 改成值传递可以不用在函数末尾恢复状态 
     * 2. 为什么要恢复状态？递归函数处理数据部分当前不应该改变上层传入的引用数据，否则会破坏上层的逻辑or数据结构。
     * */
    void _binaryTreePaths(TreeNode* root, vector<string>& paths, string& path) {
        string tmp_path = path;
        if (root == NULL) {
            return;
        }
        /* 这两句颠倒下流程会再末尾多输出个"->"，好好体会 */
        if (path.size()!=0)  path += "->";
        path += to_string(root->val);
        if (root->left==NULL && root->right==NULL) {
            paths.push_back(path);
        }

        _binaryTreePaths(root->left, paths, path);
        _binaryTreePaths(root->right, paths, path);
        /* 恢复上层传递下来的引用数据，如果改成值传递，则没有此问题 */
        path = tmp_path;
        return;
    }

public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> paths;
        string path;
        path.clear();
        _binaryTreePaths(root, paths, path);
        return paths;
    }
};