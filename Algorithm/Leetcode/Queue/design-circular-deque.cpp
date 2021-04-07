/*************************************************************************
 > File Name: design-circular-deque.cpp
 > Author: Leafxu
 > Created Time: 2021年04月07日 星期三 09时27分00秒
 ************************************************************************/

/*
  *  问题： 设计实现双端队列。
  *     你的实现需要支持以下操作：
  *     MyCircularDeque(k)：构造函数,双端队列的大小为k。
  *     insertFront()：将一个元素添加到双端队列头部。 如果操作成功返回 true。
  *     insertLast()：将一个元素添加到双端队列尾部。如果操作成功返回 true。
  *     deleteFront()：从双端队列头部删除一个元素。 如果操作成功返回 true。
  *     deleteLast()：从双端队列尾部删除一个元素。如果操作成功返回 true。
  *     getFront()：从双端队列头部获得一个元素。如果双端队列为空，返回 -1。
  *     getRear()：获得双端队列的最后一个元素。 如果双端队列为空，返回 -1。
  *     isEmpty()：检查双端队列是否为空。
  *     isFull()：检查双端队列是否满了。
  *
  *  示例：
  *  输入:   
  *  输出:   
  *  要求:  
  *
  *  完成度:
  *  解法：
  *  注解：
  *  理解:
  *               
  *  链接：https://leetcode-cn.com/problems/design-circular-deque
  */

/**
 * Your MyCircularDeque object will be instantiated and called as such:
 * MyCircularDeque* obj = new MyCircularDeque(k);
 * bool param_1 = obj->insertFront(value);
 * bool param_2 = obj->insertLast(value);
 * bool param_3 = obj->deleteFront();
 * bool param_4 = obj->deleteLast();
 * int param_5 = obj->getFront();
 * int param_6 = obj->getRear();
 * bool param_7 = obj->isEmpty();
 * bool param_8 = obj->isFull();
 */

class MyCircularDeque {
private:
    int front, rear;
    int tag;
    int size;
    int *data;
public:
    /** Initialize your data structure here. Set the size of the deque to be k. */
    MyCircularDeque(int k) {
        data = new int[k];
        for (int i=0; i<k; ++i)
        {
            data[i] = -1;
        }
        size = k;
        front = 0;
        rear = 0;
        tag = 0;
    }
    
    /** Adds an item at the front of Deque. Return true if the operation is successful. */
    bool insertFront(int value) {
        if (isFull()) {
            return false;
        }
        else {
            front = (front - 1 + size) % size;
            data[front] = value;
            tag = 1;
            return true;           
        }
    }
    
    /** Adds an item at the rear of Deque. Return true if the operation is successful. */
    bool insertLast(int value) {
        if (isFull()) {
            return false;
        } 
        else {
            data[rear] = value;
            rear = (rear + 1) % size;
            tag = 1;
            return true;
        }
    }
    
    /** Deletes an item from the front of Deque. Return true if the operation is successful. */
    bool deleteFront() {
        if (isEmpty()) {
            return false;
        }
        else {
            data[front] = -1;
            front = (front + 1) % size;
            tag = 0;
            return true;
        }
    }
    
    /** Deletes an item from the rear of Deque. Return true if the operation is successful. */
    bool deleteLast() {
        if (isEmpty()) {
            return false;
        }
        else 
        {
            rear = (rear - 1 +size) % size;
            data[rear] = -1;
            tag = 0;
            return true;
        }
    }
    
    /** Get the front item from the deque. */
    int getFront() {
        return data[front];
    }
    
    /** Get the last item from the deque. */
    int getRear() {
        return data[(rear-1+size)%size];
    }
    
    /** Checks whether the circular deque is empty or not. */
    bool isEmpty() {
        return front == rear && tag == 0;
    }
    
    /** Checks whether the circular deque is full or not. */
    bool isFull() {
        return front == rear && tag == 1;
    }
};




