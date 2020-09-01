/*************************************************************************
 > File Name: container-with-most-water/
 > Author: Leafxu
 > Created Time: 2020年09月01日 星期二 19时42分12秒
 ************************************************************************/

/*
 *  问题：给你 n 个非负整数 a1，a2，...，an，每个数代表坐标中的一个点 (i, ai) 。在坐标内画 n 条垂直线，垂直线 i 的两个端点分别为 (i, ai) 和 (i, 0)。
 *          找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
 *  示例：
 *  输入：
 *  输出：
 *  要求：
 *  说明：
 *
 *  完成度：60%
 *  解法：双指针【单一变量法】 
 *  注解：最大面积的两大因素：柱高与柱宽 【柱高随着柱宽的变化而变化，找出变化关系及应对这种变化关系的办法】
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/container-with-most-water/
 */
class Solution {
public:
    // 1. 暴力解法
    // 2. 遍历整个二维解空间。
    int maxArea1(vector<int>& height) {
        int max =  0;
        int n = height.size();
        for (int i=0; i<n; i++) {
            for (int k=0; k<n; k++) {
                int  width = height[k];
                if(height[i]<=height[k])  width = height[i];
                int containor = (k-i) * width;    
                if (containor>max) {
                    max = containor;
                }
            }
        }
        return max;
    }

    /*
    *  1. 核心思想：多因素【宽，高】为单因素【高】。
    *  2. 代码翻译：双指针法: 左右夹逼。
    *  3. 分析：降维处理 【一个维度一个循环】。
    *  4. 如何进行降维处理？【找一条直线方程，方程穿过二维解空间曲线最优解所在的点。用代码程序语言描述此直线方程】。
    */
    int maxArea2(vector<int>& height) {
        int ans = 0;        
        int i=0, j=height.size()-1;
        while(i<j){
            int area = (j-i)*min(height[i],height[j]);
            if(area>ans){
                ans = area;
            }
            if(height[i]<height[j]){
                i++;
            } else if(height[i]>height[j]){
                j--;
            } else {
                i++;j--;
            }
        }
        return ans;
    }
};
