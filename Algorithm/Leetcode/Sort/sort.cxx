#include <iostream>
using namespace std;

int paritition(int* arr, int start, int end)
{
	int val = arr[start];
	int low = start;
	int high = end;
	while (low < high)
	{
		while (low<high && arr[high]>=val) high--;
		arr[low] = arr[high];
		while (low<high && arr[low]<val) low++;
		arr[high] = arr[low];
	}
	arr[low] = val;
	return low;
}

void quick_sort(int *arr, int start, int end)
{
	if (start < end) {
		int mid = paritition(arr, start, end);
		quick_sort(arr, start, mid-1);
		quick_sort(arr, mid+1, end);
	}
}

void _merge_sort(int* arr, int* temp, int start, int end)
{
	if (start>=end) return;
	int mid = ((end -start) >> 1) +start;
	int start1 = start, end1 = mid;
	int start2 = mid+1, end2 = end;

	_merge_sort(arr, temp, start1, end1);
	_merge_sort(arr, temp, start2, end2);

	int k = start;
	while (start1<=end1 && start2<=end2)
	{
		temp[k++] = arr[start1]>arr[start2]?arr[start1++]:arr[start2++];
	}
	while (start1<=end1)
	{
		temp[k++] = arr[start1++];
	}
	while (start2<=end2)
	{
		temp[k++] = arr[start2++];
	}
	
	for (int i=start; i<=end; i++)
	{
		arr[i] = temp[i];
	}
}

void merge_sort(int* arr, int start, int end)
{
	int temp[20] = {0};
	_merge_sort(arr, temp, start, end);
}

void heapify(int* arr, int start, int end)
{
	int parent = start;
	int son = 2*parent + 1;
	while (son <= end)
	{
		// if heva right-son.
		if (son+1 <= end && arr[son]<arr[son+1]) son++;
		if (arr[parent]<arr[son]) {
			swap(arr[parent], arr[son]);
			parent = son;
			son = parent*2 + 1;
		}
		else {
			return;
		}
	}
}

void heap_sort(int* arr, int len)
{
	// 处理所有根结点，建立堆
	// 从下往上处理根节点
	// parent = k
	// lson = 2*k + 1
	// rson = 2*k + 2
	// 根结点：	arr.size()/2-1
	// 索引计算用 size();
	// 移动比较用 实值;
	for(int i=len/2-1; i>=0; i--)
	{
		cout<<"len: "<<i<<" "<<endl;
		heapify(arr, i, len-1);
	}
	for (int i=len-1; i>=0; i--)
	{
		swap(arr[0], arr[i]);
		heapify(arr, 0, i-1);
	}
}

int main()
{
	int data[] = {6,5,6,8,9,3};
	int len = sizeof(data)/sizeof(int);
	heap_sort(data, len);
	//quick_sort(data,  0, len-1);
	//merge_sort(data, 0, len-1);
	for (int i=0; i<len; i++)
		cout<<" "<<data[i];
	cout<<endl;
	return 0;
}

