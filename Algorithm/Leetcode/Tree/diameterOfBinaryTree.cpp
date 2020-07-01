/*************************************************************************
 > File Name: diameterOfBinaryTree.cpp
 > Author: Leafxu
 > Created Time: 2020年07月01 星期三 12时36分52秒
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
 *  问题：给定一棵二叉树，你需要计算它的直径长度。
 *  示例：
 *  输入：
 *  输出：   
 *  要求：  
 *  说明：一棵二叉树的直径长度是任意两个结点路径长度中的最大值。这条路径可能穿过也可能不穿过根结点。
 *
 *  完成度：60%
 *  解法：深度优先搜索，后序遍历
 *  应用：分析任意两个结点之间某些依赖于数据存储结构的特性
 *     
 *  分类：数据分析
 *  链接：https://leetcode-cn.com/problems/diameter-of-binary-tree/
 */
class Solution {
        int ans=0; // 当前最大路径和
        /*
         * 后序遍历：先进入下层，再到当前层，最后返回上层
         * 1. 下层-1层：计算左右子树最大深度
         * 2. 当前0层：根据下层返回左右子树的最大深度，算出当前节点的最长路径。
         * 3. 上层1层：返回当前层树最大深度
         * 
         * 注释：从下层获取数据，在当前层处理，返回当前层数据给上层
         * a. 下层：你想从下层获取什么数据【倾向于具体问题的场景应用】？你能获取到什么数据【倾向于数据结构理论上的分析】？
         * b. 当前层：处理结果一定要通过递归函数返回值给出去吗，有可能破坏一些什么吗？当前层返回的数据什么情况下有硬性要求？为什么？
         * c. 上层：实际上就是下层【一个循环】
         */
        int depth(TreeNode* root) {
            int L, R;
            /* 终止条件*/
            if (root==NULL) return 0;

            /* 进入下一层：求当前左右子树的深度 */
            L = depth(root->left);
            R = depth(root->right);

            /*
             * 处理当前层数据: 
             * 1. 记录数据: 【数据结构本身具有信息。比如：此处的深度】 
             * 2. 每递归一层深度加1    
             */
            ans = max(ans, L+R); 
            return max(L, R) + 1;   /* 返回上层 */
        }

    public:
        int diameterOfBinaryTree(TreeNode* root) {
            depth(root);
            return ans;
        }
};
