/*************************************************************************
 > File Name: min-stack.cpp
 > Author: Leafxu
 > Created Time: 2021年03月27日 星期六 23时43分00秒
 ************************************************************************/

/*
  *  问题： 设计一个支持 push ，pop ，top 操作，并能在常数时间内检索到最小元素的栈。
  *  push(x) —— 将元素 x 推入栈中。
  *  pop() —— 删除栈顶的元素。
  *  top() —— 获取栈顶元素。
  *  getMin() —— 检索栈中的最小元素。
  *
  *  示例：
  *  输入:   
  *  输出:   
  *  要求:  
  *
  *  完成度:
  *  解法：
  *  注解：
  *  理解:
  *               
  *  链接：https://leetcode-cn.com/problems/min-stack/
  */

class MinStack {
    stack<int> x_stack;
    stack<int> min_stack;
	public:
		MinStack() {
		min_stack.push(INT_MAX);
	}
	
	void push(int x) {
		x_stack.push(x);
		min_stack.push(min(min_stack.top(), x));
	}
	
	void pop() {
		x_stack.pop();
		min_stack.pop();
	}
	
	int top() {
		return x_stack.top();
	}
			
    int getMin() {
        return min_stack.top();
    }
};

