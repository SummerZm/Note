/*************************************************************************
 > File Name: valid-parentheses.cpp
 > Author: Leafxu
 > Created Time: 2021年03月26日 星期五 08时19分50秒
 ************************************************************************//*
 *  问题：给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s ，判断字符串是否有效。
 *      有效字符串需满足：
 *      左括号必须用相同类型的右括号闭合。
 *      左括号必须以正确的顺序闭合。
 * 
 *  示例： 
 *  输入： s = "()[]{}"
 *  输出： true
 *  要求：
 *  说明：
 *
 *  完成度：
 *  解法：
 *  注解：如果使用map对 () {} [] 进行映射可以提供程序的可读性和拓展性
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/valid-parentheses/
 */
class Solution {
    public:
        bool isValid(string s) {
            int n = s.size();
            if (n % 2 == 1) {
                return false;
            }

            unordered_map<char, char> pairs = {
                {')', '('},
                {']', '['},
                {'}', '{'}
            };
            stack<char> stk;
            for (char ch: s) {
                if (pairs.count(ch)) {
                    if (stk.empty() || stk.top() != pairs[ch]) {
                        return false;
                    }
                    stk.pop();
                }
                else {
                    stk.push(ch);
                }
            }
            return stk.empty();            
            /*
            stack<char> st;
            for (int i=0; i<s.size(); i++)
            {
                if (st.empty())
                {
                    st.push(s[i]);
                }
                else 
                {
                    char c = st.top();
                    if ( (c=='(' && s[i]==')') || (c=='[' && s[i]==']') || (c=='{' && s[i]=='}'))
                    {
                        st.pop();
                    }
                    else 
                    {
                        st.push(s[i]);
                    }
                }
            }

            if (st.empty()) 
            {
                return true;
            }
            return false;
            */
        }
};
