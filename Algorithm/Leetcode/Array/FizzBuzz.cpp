/*
 * 问题：写一个程序，输出从 1 到 n 数字的字符串表示。
 *      1. 如果 n 是3的倍数，输出“Fizz”；
 *      2. 如果 n 是5的倍数，输出“Buzz”；
 *      3.如果 n 同时是3和5的倍数，输出 “FizzBuzz”
 *
 * 完成度: 80% - 2
 * 前置条件: NULL.
 * 注解: NULL
 * 链接: https://leetcode-cn.com/problems/fizz-buzz/
 */
class Solution {
public:
    vector<string> fizzBuzz(int n) {
        vector<string>  containor;
        for (int i=1; i<=n; i++) {
            if  (i%3==0 && i%5==0)  {
                containor.push_back("FizzBuzz");
            }
            else if (i%3==0) {
                containor.push_back("Fizz");
            }
            else if (i%5==0) {
                containor.push_back("Buzz");
            }
            else {
                containor.push_back(to_string(i));
            }
        }
        return containor;
    }

    vector<string> fizzBuzz2(int n) {
        vector<string> containtor;
        map<int, string> dict;
        dict[3] = "Fizz";
        dict[5] = "Buzz";
        for (int i=1; i<=n; i++) {
            string ret;
            for (auto f:dict) {
                if (i%f.first==0) {
                    ret += f.second;
                }
            }
            if (ret.empty()) {
                ret = to_string(i);
            }
            containtor.push_back(ret);
        }
        return  containtor;
    }
}; 
