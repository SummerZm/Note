/*
 * 问题：给定两个字符串 s 和 t ，编写一个函数来判断 t 是否是 s 的字母异位词.  长度一致才是字母异位词
 * 本质：对字符进行排序然后比较
 * 考察：对API, 语言的熟悉度
 * 完成度： 40%
 * 链接：https://leetcode-cn.com/problems/valid-anagram/ 
 */
class Solution {
public:
    // C++ API 的熟练度
    bool isAnagram1(string s, string t) {
        sort(s.begin(), s.end());
        sort(t.begin(), t.end());
        return s==t;
    }
    
    // 最优：抽出特征，进行特征比较
    // 特征分析：字符串的长度， 字符出现的个数.
    bool isAnagram2(string s, string t) {
        // 特征比较
        if (s.size() != t.size()) return false;
        // 特征模型结构: 英文字符出现的次数 
        vector<int> count_s(26, 0);
        vector<int> count_t(26, 0);
        for (int i = 0; i < s.size(); ++i){
            // 特征分析
            count_s[s[i] - 'a']++;
            count_t[t[i] - 'a']++;
        }
        //  特征比较
        return count_s == count_t;
    }
};
