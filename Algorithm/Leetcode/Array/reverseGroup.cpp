/*************************************************************************
 > File Name: reverseGroup.cpp
 > Author: Leafxu
 > Created Time: 2021年01月17日 星期日 12时49分11秒
 ************************************************************************/
/*
 *  问题：
 *     给你一个链表，每 k 个节点一组进行翻转，请你返回翻转后的链表。
 *     k 是一个正整数，它的值小于或等于链表的长度。
 *     如果节点总数不是 k 的整数倍，那么请将最后剩余的节点保持原有顺序。
 * 
 *  示例：
 *  给你这个链表：1->2->3->4->5
 *  当 k = 2 时，应当返回: 2->1->4->3->5
 *  当 k = 3 时，应当返回: 3->2->1->4->5
 *
 *  说明：
 *  你的算法只能使用常数的额外空间。
 *  你不能只是单纯的改变节点内部的值，而是需要实际进行节点交换。
 *
 *  完成度：90%
 *  解法：头节点简单应用
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/reverse-nodes-in-k-group
 */
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    /* Reverse linked-list and returnt new head/tail */
    pair<ListNode*, ListNode*> xuReverse(ListNode* head, ListNode* tail) {
		/* prev is good.*/
        ListNode* prev = tail->next;
        ListNode* cur = head;
        while (prev != tail) {
            ListNode* nex = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nex;
        }
        return {tail, head};
    }

    ListNode* reverseKGroup(ListNode* head, int k) {
        /* Power head node */
        ListNode* eye = new ListNode(0);
        eye->next = head;
        ListNode* pre = eye;
        while (head) {
            ListNode* tail = pre;
            for (int i=0; i<k; ++i) {
                tail = tail->next;
                if (tail == NULL) {
                    return eye->next;
                }
            }
            /* Store the tail->next */
            ListNode* nex = tail->next;
            pair<ListNode*, ListNode*> sub = xuReverse(head, tail);
            head = sub.first;
            tail = sub.second;
            /* Reconnext the reversed-sub-linked-list to The trunk */
            pre->next = head;
            tail->next = nex;
            /* Update pointer */
            pre = tail;
            head = tail->next;
        }
        return eye->next;
    }
};
