## **数组问题**

### **A. 数组创建相关问题**
```c
// .....
ListNode *head, *tail;
if (!head) {
    head = tail = new ListNode(val);
}
else {
    tail->next = new ListNode(val);
    tail = tail->next;
}
// .....
```