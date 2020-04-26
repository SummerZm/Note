/*
 *  问题：给定一个字符串数组，将字母异位词组合在一起。字母异位词指字母相同，但排列不同的字符串.
 *  示例：
 *  输入: ["eat", "tea", "tan", "ate", "nat", "bat"].
 *  输出:
 *    [
 *         ["ate","eat","tea"],
 *         ["nat","tan"],
 *         ["bat"]
 *    ]
 *
 *  完成度：20%
 *  我的：遍历数组，逐个比较.
 *  解法：[对输入数据进行提取同类项]
 *  1. 当且仅当它们的字符计数（每个字符的出现次数）相同时，两个字符串是字母异位词.
 *  2. 当且仅当它们的排序字符串相等时，两个字符串是字母异位词. 
 *  3. 链接：https://leetcode-cn.com/problems/group-anagrams/
 */
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> result;
        // 生成的特征数据放在哪里？[生成存放特征数据]
        vector<string> feature;
        int strsize = strs.size();
        for (int i=0; i<strsize; i++) {
            feature.push_back(sort(strs[i].begin(), strs[i].end()));
        }
        // 对特征数据进行处理 [找出特征值相同的下标 - 找出数组中相同元素的下标]
        //  1  - a
        //  2  - c      a -> 1, 4
        //  3  - b  =>  b -> 3
        //  4  - a      c -> 2, 5
        //  5  - c
        vector<string> featureKey;    
        for (int i=0; i<strsize; i++) {
            featureSet.add(strs[i]);
        }

        vector<vector<int>> featureVal;
        for (int i=0; i<strsize; i++) {
            for (int n=0; n<featureKey.size(); n++) {
                if (strs.[i]==featureKey[n]) {
                    featureVal[n].push_back[strs[i]];
                }
            }
        }
        return featureVal;
    }
};
