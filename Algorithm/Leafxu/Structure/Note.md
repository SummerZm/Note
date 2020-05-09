### <b>环形数组 - 环形换座问题</b>  ###
-  Mod运算 & 环形数组回绕
```C++
    /*
     *  If pos > arr.size(), then roll back.
     */
    int getIndex(vector<int> arr, int pos) {
        return = pos % arr.size();
    }
```

### <b>完全二叉数组树</b>  ###
- 数组下标0为根节点，左子树 i*2+1, 右子树 i*2+2
- 数组下标1为根节点，左子树 i*2, 右子树 i*2+1