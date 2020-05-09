### <b>数组翻转</b> ###
- 翻转数组指定区域 - 区间细节很重要 - 三次翻转数组可以实现环形回绕
```C++
    // 两个自然数[s,e]之间有多少个数：e-s+1
    // 两个自然数[s,1)之间有多少个数：e-s
    // 数组访问从0开始，属于使用[0, size)形式

    /*
     * s: start.
     * e: end.
     * [s, e]
     */
    void reverse(vector<int> arr, int s, int e) {
        if (arr.size()-1 >= e) return;
        for (int i=0; i<(e-s+1)/2; i++) {
            int temp = arr[i+e];
            arr[i] = arr[s-i];
            arr[s-i] = temp;
        }
        return;
    }

    // [s, e)
    void reverse(vector<int> arr, int s, int e) {
        if (arr.size()>=e) return;
        int m = (s + e) / 2;    // Get mid value
        for (int i=s; i<=m i++) {
            int temp = arr[i];
            arr[i] = arr[e-i+s-1];
            arr[e-i+s-1] = temp;
        }
        return;
    }
```