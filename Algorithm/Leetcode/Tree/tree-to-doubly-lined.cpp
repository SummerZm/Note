/*************************************************************************
 > File Name: tree-to-doubly-lined.cpp
 > Author: Leafxu
 > Created Time: 2020年07月26日  9:54:23
 ************************************************************************/
/*
  *  问题：将二叉树转换为双链表
  *  示例：
  *  输入：
  *  输出：   
  *  要求： 
  *  说明：
  *  完成度：60%
  *  解法：中序遍历 [修改构造原树] 利用头节点
  *  应用： 
  *     
  *  分类：数据构造 
  *     1. 如果在栈上分配内存，则不会出现内存泄漏情况]
  *     
  *  链接：https://leetcode-cn.com/problems/convert-binary-search-tree-to-sorted-doubly-linked-list/
  */
 /*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;

    Node() {}

    Node(int _val) {
        val = _val;
        left = NULL;
        right = NULL;
    }

    Node(int _val, Node* _left, Node* _right) {
        val = _val;
        left = _left;
        right = _right;
    }
};
*/
class Solution {
public:
    Node* curr;
    Node* treeToDoublyList(Node* root) {
        Node head(0, nullptr, nullptr);
        if (root!=nullptr) {
            curr = &head;
            inorder(root);
            // 摘掉头节点
            curr->right = head.right;
            head.right->left = curr;
        }
        return head.right;
    }  

    void inorder(Node* root) {
        if (root==nullptr) return;
        inorder(root->left);
        //  挂当前节点
        curr->right = root;
        root->left = curr;
        curr = root;
        inorder(root->right);
    }
};
