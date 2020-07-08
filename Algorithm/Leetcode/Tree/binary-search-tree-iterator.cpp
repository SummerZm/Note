/*************************************************************************
 > File Name: binary-search-tree-iterator.cpp
 > Author: Leafxu
 > Created Time: 2020年07月08日 星期三 19时32分12秒
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
 * Your BSTIterator object will be instantiated and called as such:
 * BSTIterator* obj = new BSTIterator(root);
 * int param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */

/*
 *  问题：实现一个二叉搜索树迭代器。你将使用二叉搜索树的根节点初始化迭代器。调用 next() 将返回二叉搜索树中的下一个最小的数。
 *  示例：
 *  输入：
 *  输出：
 *  要求：
 *  说明：
 *
 *  完成度：60%
 *  解法：1. 开个数组中序遍历并将节点存一遍，在逐个返回。 2. 受控递归[自己控制递归栈]
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/binary-search-tree-iterator/
 */
class BSTIterator {
private:
    /*int curr;
    vector<TreeNode*> quickIndex;

    void inOrder(TreeNode* root) {
        if (root==nullptr) return;
        inOrder(root->left);
        quickIndex.push_back(root);
        inOrder(root->right);
        return;  
    }*/

    stack<TreeNode*> quickIndex;
    void inOrderLeft(TreeNode* root) {
        while(root) {
            quickIndex.push(root);
            root = root->left;
        }
    }

public:
    BSTIterator(TreeNode* root) {
        /*curr = 0;
        inOrder(root);*/
        inOrderLeft(root);
    }
    
    /** @return the next smallest number */
    int next() {
        /*return quickIndex[curr].val;*/
        TreeNode* min = quickIndex.top();
        quickIndex.pop();
        if (min->right!=NULL) {
            inOrderLeft(min->right);
        }
        return min->val;
    }
    
    /** @return whether we have a next smallest number */
    bool hasNext() {
        /*return curr<quickIndex.size()?true:false;*/
        return !quickIndex.empty();
    }
};