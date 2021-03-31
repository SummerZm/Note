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
	*  5. 尝试着从树的思维去解决这个问题 【2021-01-02】
	*     a. 问题转化：如何生成树空间并在生成的过程中找到最大值？
	*     b. 注解1：树空间中的值是指面积
	*     c. 如何确定树空间的根：
	*        -- when x=max; count(area)=1
	*        -- when x=max-1; count(area)=2
	*        -- when x=max-2; count(area)=4
	*        -- 注解: x=max-1 表示第二大，依次类推...
	*        -- 由此可得：x从大到小的变化，可作为树空间生长的方向，我们可以在生长的过程中进行大小判断剪枝
	*        -- 此时，x的个数便是树的深度
    *        
    * 6. 5中的说法是有问题的。
    *     
    * 7. 把数组当成环x宽度取法个数如下： n*n 
    *     宽度  个数
    *       4： 4
    *       3:  4
    *       2:  4
    *       1:  4
    * 
    * 8. 把数组当成线条：
    *     宽度  个数
    *       4： 1
    *       3:  2
    *       2:  3
    *       1:  4
    * 
    *  9. 不断的计算当前(x=4,3,2,1)层次的面积最大值。之所以能计算出来是因为他们自身上层的隐性限制
    *      然后更新最大值
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
