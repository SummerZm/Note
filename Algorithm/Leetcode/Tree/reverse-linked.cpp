/*************************************************************************
 > File Name: reverse-linked.cpp
 > Author: Leafxu
 > Created Time: 2021年01月05日 星期二 12时16分52秒
 ************************************************************************/
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
/*
  *  问题：反转链表
  *  示例：
  *  输入：
  *  输出：   
  *  要求：  
  *  说明：
  *
  *  完成度：80%
  *  解法：
  *  应用： 
  *     
  *  分类：数据构造
  *  链接：https://leetcode-cn.com/problems/reverse-linked-list/
  */
class Solution {
public:
    ListNode* reverseList(ListNode* head) { 
        return _reverseList(head);
    }

    // 递归
    ListNode* _reverseList(ListNode* node) {
        ListNode* tmp=nullptr;
        if (!node || !node->next) {return node;}
        tmp = _reverseList(node->next);
        node->next->next = node;    // 类型这个种多重next的用法容易断链
        node->next = nullptr;       
        return tmp;                 // 把最深处的值返回出来
    }

    // 双指针法
    // 没有用心的投进去，所以效率很低下.
    // 很少有人告诉你，用心不是状态，是一门能力，它有迹可循并且需要学习.
    ListNode* reverseList2(ListNode* head) {
        // if (!head) return head;      // [v1]error
        ListNode *pre=nullptr, *cur=head;   // 头结点的使用避免了多重next,减低错误概率，简化代码逻辑【下面有问题版本对比】
        // while (cur->next) {          // [v1]errors
        while (cur) {                   // 明确修改的是数据还是指针指向【Important】
            ListNode* tmp = cur->next;  // save
            cur->next = pre;            // modify
            pre = cur;                  // move
            cur = tmp;                  // move
        }
        // return cur; [v1]error
        return pre;

        /*
        if (!head || !head->next) return head;
        ListNode* cur = head;
        while (cur->next) {
            ListNode* tmp = cur->next->next;  // [v1]error: 中间环节断开了
            cur->next->next = head;
            head = cur->next;
            cur->next = tmp;                 // [v1] error: 此处改动断开了链表
        }
        return cur
        */
    }


    ListNode* reverseList3(ListNode* head) {
		ListNode* tail = head;
		if ( !head || !head->next) return head;
		while ( tail->next ) tail = tail->next;
		head->next = _reverseList2(head, head->next);
		return tail;
	}

    ListNode* _reverseList2(ListNode *parent, ListNode *node) 
    {
        if(node->next==NULL) {
            node->next = parent;
            return NULL;
            // return parent [v1]error: 这里返回parent给上层的node,已经和本层的node不是同一个了;
        }
        _reverseList(node, node->next);
        node->next = parent;
        return NULL;
        // node->next = _reverseList(node, node->next); 
        // return parent; [v1]error:
    }
};


