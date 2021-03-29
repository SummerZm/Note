/*************************************************************************
 > File Name: largest-rectangle-in-histogram.cpp
 > Author: Leafxu
 > Created Time: 2021年03月29日 星期一 10时10分05秒
 ************************************************************************/
/*
 *  问题：柱状图中最大的矩形.
 *  示例：
 *      给定 n 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为 1 。
 *      求在该柱状图中，能够勾勒出来的矩形的最大面积。
 * 
 *  输入: [2,1,5,6,2,3]
 *  输出: 10
 *
 *  完成度：50%
 *  我的：如果代码是顺序，循环，分支；那么咱们的基本功应该有如何玩转循环这项吧。 画画图看下，循环的玩法。
 *          将程序转化为在坐标轴上的玩法。我觉得可以形成一套系统。
 *          坐标轴的组成很简单，但是却可以演化出好多数学。比如：解析几何。
 *          将程序语言翻译成数学语言
 *  解法：
 *  
 *  链接：https://leetcode-cn.com/problems/largest-rectangle-in-histogram/
 */
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        /*
        int n = heights.size();
        int ans = 0;
        // 枚举左边界
        for (int left = 0; left < n; ++left) {
            int minHeight = INT_MAX;
            // 枚举右边界
            for (int right = left; right < n; ++right) {
                // 确定高度
                minHeight = min(minHeight, heights[right]);
                // 计算面积
                ans = max(ans, (right - left + 1) * minHeight);
            }
        }
        return ans;
        */

        /*
        int n = heights.size();
        int ans = 0;
        for (int mid = 0; mid < n; ++mid) {
            // 枚举高
            int height = heights[mid];
            int left = mid, right = mid;
            // 确定左右边界
            //【Leafxu】 向左右找比当前高的。找不到就不用找了
            while (left - 1 >= 0 && heights[left - 1] >= height) {
                --left;
            }
            while (right + 1 < n && heights[right + 1] >= height) {
                ++right;
            }
            // 计算面积
            ans = max(ans, (right - left + 1) * height);
        }
        return ans;
        */

        /*
        int n = heights.size();
        vector<int> left(n), right(n);
        
        stack<int> mono_stack;
        for (int i = 0; i < n; ++i) {
            while (!mono_stack.empty() && heights[mono_stack.top()] >= heights[i]) {
                mono_stack.pop();
            }
            left[i] = (mono_stack.empty() ? -1 : mono_stack.top());
            mono_stack.push(i);
        }

        mono_stack = stack<int>();
        for (int i = n - 1; i >= 0; --i) {
            while (!mono_stack.empty() && heights[mono_stack.top()] >= heights[i]) {
                mono_stack.pop();
            }
            right[i] = (mono_stack.empty() ? n : mono_stack.top());
            mono_stack.push(i);
        }
        
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            ans = max(ans, (right[i] - left[i] - 1) * heights[i]);
        }
        return ans;
        */

        /*
        int n = heights.size();
        vector<int> left(n), right(n, n);
        
        stack<int> mono_stack;
        for (int i = 0; i < n; ++i) {
            while (!mono_stack.empty() && heights[mono_stack.top()] >= heights[i]) {
                right[mono_stack.top()] = i;
                mono_stack.pop();
            }
            left[i] = (mono_stack.empty() ? -1 : mono_stack.top());
            mono_stack.push(i);
        }
        
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            ans = max(ans, (right[i] - left[i] - 1) * heights[i]);
        }
        return ans;
        */
    }
};