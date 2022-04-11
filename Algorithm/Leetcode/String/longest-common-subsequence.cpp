// 	不同的子序列
int numDistinct(string s, string t) {
	int n = s.size();
	int m = t.size();
	if (m>n) return 0;
	vector<vector<int>> dp(n+1, vector<int>(m+1));
	for (int i=0; i<=n; i++) dp[i][0] = 1;	// import 
	for (int i=1; i<=n; i++) {
		for (int j=1; j<=m; j++) {
			if (s[i-1]==t[j-1]) dp[i][j] = dp[i-1][j-1] + dp[i-1][j];
			else dp[i][j] = dp[i-1][j];
		}
	}
	return dp[n][m];
}

// 最长公共子数组
int findLength(vector<int> nums1, vector<int> nums2)
{
	int ans = 0;
	vector<vector<int>> dp(nums1.size()+1, vector<int>(nums2.size()+1, 0));
	for (int i=1; i<=nums1.size(); i++)
	{
		for (int j=1; j<=nums2.size(); j++)
		{
			if (nums1[i-1]==nums[j-1]) dp[i][j] = dp[i-1][j-1] + 1;
			ans = max(ans, dp[i][j]);
		}
	}
	return ans;
}

// kmp算法
void make_next(string text, int* next)
{
	int m = strlen(text);
	next[0] = 0;
	for (int i=1, k=0; i<m; i++)
	{
		while (i>0 && text[i]!=text[k])
		{
			k = next[i-1];
		}
		if (text[i]=text[k]) k++;
		next[i] = k;
	}
}

int kmp(string text1, string text2, int* next)
{
	int n = strlen(text1);
	int m = strlen(text2);
	make_next(text2, next);
	for (int i=0, j=0; i<n; i++) 
	{
		while (j>0 && text1[i]!=text2[j]) {
			j = next[j-1];
		} 
		if (text1[i]==text2[j]) j++;
		ir (j==m-1) return i-j+1;
	}
	return 0;
}

//  最长公共子序列
//	1. dp表的初始化
//	2. 表起点的选择
int longeet-common-subsequence(string text1, string text2)
{
	int n = text1.size();
	int m = text2.size();
	vector<vector<int>> dp(n+1, vector<int>(m+1));
	for (int i=1; i<=n; i++)
	{
		for (int j=1; j<=m; j++)
		{
			if (text1[i]==text2[j]) dp = dp[i-1][j-1] + 1;
			else dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
		}
	}
	return dp[n][m];
}
// 最长回文字串
// 1. 中心拓展算法 - 起点和长度的计算方法
pair<int, int> expandRoundCenter(string s, int left, int right)
{
	while (left>=0 && right<=s.size() && s[left--] == s[right++]){}
	return {left+1, right-1};
}

string longestPalindrome(string s)
{
	int start=0, end=0;
	for (int i=0; i<s.size(); i++)
	{
		pair<int, int> p1 = expandRoundCenter(s, i. i);
		pair<int, int> p2 = expandRoundCenter(s, i, i+1);
		if (p1.second - p1.first > end - start) 
		{
			start = p1.fisrt;
			end = p1.second;
		}
		if (p2.second - p2.first > end - start)
		{
			start = p2.fisrt;
			end = p2.second;
		}
	}
	return s.substr(start, end-start+1);
}
// 最长公共前缀
string longestCommonPrefix(vector<string>& strs) 
{
	int len = strs.size();
	if (len==0) { return "";}

	string ans;
	int minLen = strs[0].size();
	for (int i=1; i<len; i++)
	{
		minLen = min(minLen, strs[i].size());
	}

	for (int i=1; i<minLen; i++)
	{
		char ch = strs[0][i];
		for (int j=1; j<len; j++)
		{
			if (strs[j][i]!=ch) return ans;
		}
		ans += ch;
	}
	return ans;
}
// 反转字符串
string reverseWords(string s) 
{
	int len = s.size();
	stack<string> str_st;
	string str, ans;
	for (int i=len-1; i>=0; i--)
	{
		if (s[i]==' ') {
			str_st.push(str);
			str.clear();
		}
		else str += s[i];
	}
	str_st.push(str);
	// error: for(int i=0; i<str_st.size(); i++)
	while (!str_st.empty())
	{
		ans += str_st.top() + " ";
		str_st.pop();
	}
	return ans.substr(0, ans.size()-1);
}














