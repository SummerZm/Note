/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
/*
 *  问题：将两个升序链表合并为一个新的 升序 链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。 
 *  示例：
 *  输入：l1 = [1,2,4], l2 = [1,3,4]
 *  输出：[1,1,2,3,4,4]
 * 
 *  完成度：20%
 *  解法：
 *      1. 递归函数四大问题
 *          a. 终止问题
 *          b. 最小问题 - 递归函数的主要工作
 *          c. 进入路径问题 - 如何给递归函数传参数
 *          d. 回溯路径问题 - 如何接住递归函数的返回
 * 
 *      2. 递归函数的三部态
 *          a. 第一部分【终止态】： 终止条件
 *          b. 第二部分【迭代态】： 选路问题 - 在当前轮中选择参与下一轮递归的内容
 *          c. 第三部分【现在态】： 最小子问题
 *
 *      3. 简单递归的小技巧
 *          a. 根据最终结果确定最小子问题
 *            - 最终结果是升序链表 
 *            - 基于最小子问题是在递归的最深处开始执行往回走事实. 最小子问题是：返回两个数中的最小值.
 *
 *          b. 根据最小子问题确定行进回溯问题
 *            - 最终结果是升序链表 
 *            - 行进路径: 最小子问题是两个数中的最小值,所以行进目的是：用去除当前轮最小值后剩下的数据,进行下一轮子问题求值
 *            - 回溯路径: 最小子问题是两个数中的最小值,上一轮的较大值节点进入了当前轮的求值,所以能用来接住返回的只有上一轮的较小值
 *                  【回溯是指将当前轮子问题结果，返回给上一轮】
 * 
 *  总结：0-1问题递归（类似树思想）的方式去解决 - 这是一大类问题的思路【很重要】
 *  拓展：合并三个排序数组呢？
 *  链接：https://leetcode-cn.com/problems/merge-two-sorted-lists/   
 */
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        // 终止条件
        if (l1==NULL) return l2;
        if (l2==NULL) return l1;
        // 进入递归路径选择 【如何给递归函数传参数】
        // 留下大的,小的往前挪一个节点.
        // 因为数组是从小往大排的，总能重新选路的机会.
        if (l1->val>l2->val) {
            // 退出递归路径选择 【如何接住递归函数的返回】
            // 返回小的,留下大的.【与进入的路径相反】
            l2->next = mergeTwoLists(l1, l2->next);
            return l2;
        }
        else {
            l1->next = mergeTwoLists(l1->next, l2);   
            return l1;
        }

        // 当前工作:从最终期待的结果,可知函数主要工作是返回小的.
        // 为了代码工整可读, 才需要另起一个重复判断, 一般情况下,选路时会包含子问题的求解
        //return l1->val>l2->val?l2:l1;
    }

    ListNode* mergeTwoLists1(ListNode* l1, ListNode* l2) {
        ListNode *head=NULL, *tail=NULL;
        while (l1 && l2) {
            ListNode *temp= NULL;
            if ( l1->val < l2->val) {
                temp = l1;
                l1 = l1->next;
            }
            else {
                temp = l2;
                l2 = l2->next;
            }

            if (head==NULL) {
                head = temp;
                tail = temp;
            }
            else {
                tail->next = temp;
                tail = tail->next;
            }
        }
        if (tail) {
            tail->next = l1==NULL?l2:l1;
        }
        else {
            head = l1==NULL?l2:l1;
        }
        return head;
    }
};

   