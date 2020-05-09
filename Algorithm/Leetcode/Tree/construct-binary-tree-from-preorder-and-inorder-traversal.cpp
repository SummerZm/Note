/*************************************************************************
 > File Name: construct-binary-tree-from-preorder-and-inorder-traversal.cpp
 > Author: Leafxu
 > Created Time: 2020年05月07日 星期四 10时32分12秒
 ************************************************************************/
/*
 *  问题：根据一棵树的前序遍历与中序遍历构造二叉树。
 *  示例：
 *  输入：
 *  输出：   
 *  要求：  
 *  说明：
 *
 *  完成度：40%
 *  解法：递归/迭代
 *  注解：使用Map加速，用先序创建结点，用中序构建树形关系.
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal
 */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
     
class Solution {
	public:
		/* 
		 递归分治解法 
		 前序遍历，先构造当前节点再构造左右子树节点
		*/
		TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
			if(preorder.size()==0||inorder.size()==0)
				return NULL;

			map<int,int> m;	// 加速
			for(int i=0;i<inorder.size();++i){
				m[inorder[i]]=i;
			}
			int cur=0;//表示这是第几个节点
			return build(preorder,0,inorder.size()-1,m,cur);
		}

		/* 左闭右闭区间 */
		TreeNode* build(vector<int>& preorder, int is,int ie,map<int,int>& m, int& cur){
			if(ie<is)
			return nullptr;
			
			TreeNode* root=new TreeNode(preorder[cur]);//使用第cur个点生成节点
			root->left=NULL;
			root->right=NULL;

			int i=m[preorder[cur++]];//当前节点在中序遍历里的位置

			root->left=build(preorder,is,i-1,m,cur);
			root->right=build(preorder,i+1,ie,m,cur);
			return root;
		}

		/* 
		 迭代法是自底向上的，每次迭代不要求从上面传下来的值
		 反而需要先解决子问题后才可以解决父问题
		 换句话说就是得先构建完最远的叶子，然后一步一步往上回溯，最后回到根节点
		 所以迭代法不需要保存mostLeft和mostRight节点，也不需要知道下标的位置       
		*/
    	TreeNode* builderStack(vector<int>& preorder, vector<int>& inorder) {
            int preIndex = 0;

            // 获取最根节点值 并构建该节点
            int rootVal = preorder[preIndex];
            ++preIndex;
            TreeNode* root = new TreeNode(rootVal);
        
            // 根节点入栈，迭代构建左右子树
            stack<TreeNode*> travelStack;
            travelStack.push(root);
            
            // 迭代终止条件是preorder所有节点遍历完成
            for (int preIndex = 1, inIndex = 0; preIndex < preorder.size(); ++preIndex) {
                TreeNode* tmp = nullptr;
                while (!travelStack.empty() && travelStack.top()->val == inorder[inIndex]) {
                    // 循环条件：栈不为空，且栈顶刚好是中序的第i个节点
                    // 中序定义：一颗完整的树（左右孩子均有）第一个节点是左节点 然后是根节点，最后是右节点
                    // 如果栈顶的元素是中序的第i个元素，那么证明已经构建到当前的位置了
                    // 换句话说就是左子树已经全部构建完毕了，那么接下来的节点一定是右子树
                    // 否则的话不会进入循环，tmp是空值，那么证明这个节点的左子树还没有构建完成
                    ++inIndex;
                    tmp = travelStack.top();
                    travelStack.pop();
                }
            
            // 获取先序的下一个节点值，构建节点
            TreeNode* cur = new TreeNode(preorder[preIndex]);
            
            // 先序节点获取的顺序是根-左-右的，tmp的取值在while循环内已经阐述过了
            if (tmp == nullptr) {
                // 如果是空值，那么证明栈顶元素的左子树还没有构建完成，继续构建左子树
                travelStack.top()->left = cur;
            } else {
                // 否则栈顶的左子树已经完成了，而刚刚构建的cur是栈顶的右子叶
                tmp->right = cur;
            }
            // 将这个子叶压栈，循环检查
            travelStack.push(cur);
        }
        return root;
    }
};
