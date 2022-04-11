#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void make_next(const char* pattern, int* next)
{
	int len = strlen(pattern);
	int p, k;
	next[0] = 0;
	// 确定循环处理的对象主体
	// 一个k统计次数，一个p维护next数组写的位置
	for (p=1, k=0; p<len; p++)
	{
		while (k>0 && pattern[k]!=pattern[p]) {
			k = pattern[k-1];
		}
		if (pattern[p]==pattern[k]) k++;
		// 确定循环的目的
		next[p] = k;
	}
}

int kmp(const char* text, const char* pattern, int* next)
{
	int n = strlen(text);
	int m = strlen(pattern);

	make_next(pattern, next);
	for (int p=0, k=0; p<n; p++)
	{
		while (k>0 && text[p]!=pattern[k])
		{
			k = next[k-1];
		}
		if (text[p]==pattern[k]) k++;
		if (k==m) return p-k+1;
	}
	return -1;
}

int main() {
    char *text = "abcabcabcabcabcd";
    char *pattern = "abcabcd";
	int next[20] = {0};
	int idx = kmp(text, pattern, next);
	printf("match pattern: %d\n", idx);
}
