/*************************************************************************
 > File Name: climbing-stair.cpp
 > Author: Leafxu
 > Created Time: 2020年09月15日 星期二 11时19分50秒
 ************************************************************************//*
 *  问题：n阶楼梯，每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶呢？
 *  示例：
 *  输入：
 *  输出：
 *  要求：
 *  说明：
 *
 *  完成度：30%
 *  解法：1. 动态规划，2. 矩阵快速幂，3. 通项公式
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/climbing-stairs
 */
class Solution {
    public:
    //法2： int res;
    int climbStairs(int n) {
    
        /* 法1
        int res;
        _climbStairs(n, res);
        return res;
        */
        
        /* 法2
        _climbStairs(n);
        return res;
        **/

        /* 法3: 动态规划 - 斐波那契 
                p  q  res
           r0 = 0, 0, 0
           r1 = 0, 0, 1 
           r2 = 1, 1, 2 
           r3 = 1, 2, 3    
           r4 = 2, 3, 5    
        * * */
        int p=0, q=0, res=1;
        for (int i=1; i<=n; i++) {
            p = q;
            q = res;
            res = p + q;
        }
        return res;
    }

    /*
    解法一：
    失败的原因：通过传参计数，递归回到上层时，上层状态与下层的处理结果发生冲突覆盖。
    void _climbStairs(int m, int& res) {
        if (m==0) {
            res++;
            return;
        }
        if (m-1>=0) _climbStairs(m-1, res);
        if (m-2>=0) _climbStairs(m-2, res);
        return;
    }
    */

    /* 解法二：超时*/
    /*void _climbStairs(int m) {
        if (m==0) {
            res++;
            return;
        }
        if (m-1>=0) _climbStairs(m-1);
        if (m-2>=0) _climbStairs(m-2);
        return;
    }*/  
};


