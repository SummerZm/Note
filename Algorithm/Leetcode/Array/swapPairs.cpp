/*************************************************************************
 > File Name: swapPairs.cpp
 > Author: Leafxu
 > Created Time: 2021年01月06日 星期二 12时35分03秒
 *************************************************************************
/*
 *  问题：两两交换链表中的节点
 *  示例：
 *  输入：
 *  输出：   
 *  说明：你不能只是单纯的改变节点内部的值。
 *  要求：  
 *
 *  完成度：50%
 *  解法： 1. 递归. 2. 非递归
 *  注解：
 *  分类：数据处理
 *  链接：https://leetcode-cn.com/problems/swap-nodes-in-pairs/
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
    // 考虑[1,2,3], [], [1] [1,2] 输入情况
    ListNode* swapPairs(ListNode* head) {
        ListNode* cur = head;
        ListNode* pre = head?head->next:NULL;
        ListNode* nHead = NULL;
        if (!cur || !pre) {
            return cur;
        }
        else {
            nHead = pre;
        }

        while (cur) {
            ListNode* tmp = pre->next;   // save

            pre->next = cur;             // modify
            if (tmp) {
                if (tmp->next) {             // For [1,2,3,4] case
                    cur->next = tmp->next;   // modify
                    pre = tmp->next;         // move 
                    cur = tmp;               // move   
                }
                else {                      // For [1,2,3] case
                    cur->next = tmp;        // modify
                    cur = tmp;              // move
                    break; 
                }
            }
            else {                          // The last loop
                cur->next = NULL;           // modify
                break;
            }
        }
        return nHead;
    }

    // 递归
    ListNode* swapPairs1(ListNode* head) {
        if (head==NULL || head->next==NULL) {
            return head;
        }
        ListNode* nHead = head->next;
        head->next = swapPairs1(nHead->next);
        nHead->next = head;
        return head;
    }
};