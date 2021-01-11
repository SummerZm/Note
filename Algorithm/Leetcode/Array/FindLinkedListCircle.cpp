/*************************************************************************
 > File Name: container-with-most-water/
 > Author: Leafxu
 > Created Time: 2021年01月11日 星期一 12时52分07秒
 ************************************************************************/
/*
 *  问题：判断链表中有环并且找到环口
 *  示例：
 *  输入：
 *  输出：
 *  要求：
 *  说明：假设相遇时的状态，构造等量关系，进而整体把握问题。【在此之前需要证明一定会相遇】
 *
 *  完成度：80%
 *  解法：哈希表|双指针
 *  注解：证明快慢指针在有环情况下相遇和确定环口.
 *  分类：数据处理
 *  链接：https://leetcode.com/problems/linked-list-cycle-ii
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        // 为何慢指针第一圈走不完一定会和快指针相遇： 
        // 第一步，快指针先进入环
        // 第二步：当慢指针刚到达环的入口时，快指针此时在环中的某个位置(也可能此时相遇) 
        // 第三步：设此时快指针和慢指针距离为x，若在第二步相遇，则x = 0；
        // 第四步：设环的周长为n，那么看成快指针追赶慢指针，需要追赶n-x；
        // 第五步：快指针每次都追赶慢指针1个单位，设慢指针速度1/s，快指针2/s，那么追赶需要(n-x)s 
        // 第六步：在n-x秒内，慢指针走了n-x单位，因为x>=0，则慢指针走的路程小于等于n，即走不完一圈就和快指针相遇

        /*
            环口计算：
            环口到相遇点的距离：b
            相遇点距离环口：c            
            起点到环口：a
            慢针运动距离：a+b
            快针运动距离：a+n(b+c)+b                
            等量关系：【2(a+b) = a+n(b+c)+b】==>【a = n(b+c)-b = (n-1)(b+c)+c】
            因为b+c 等于环长，所以 a = c.据此，相遇之后，同时在链头起一个指针和慢针同速移动，会相遇环口
        */
        ListNode *fast,*slow,*pos;
        fast=slow=head;
        if (!head || !head->next) return nullptr;
        fast = fast->next->next;
        slow = slow->next;
        while (fast && slow) {
            if (fast==slow) {   // 有环
                pos=head;
                break;
            }

            if (!fast->next) {return nullptr;}
            fast = fast->next->next;
            slow = slow->next;
        }
       
        while (pos) {   // 环口
            if (pos==slow) return pos;  
            slow = slow->next;
            pos = pos->next;
        }
        return nullptr;
        /* v1: Error
        ListNode *fast=*slow=*pos=head;
        if (!head || !head->next) return 0
        fast = faset->next->next;
        slow = slow->next;
        while (fast || slow) {
            if (fast==slow) {
                break;
            }
            if (fast->next==NULL) return NULL;
            fast = fast->next->next;
            slow = slow->next;
        }
       
        while (pos!=slow) { // Err here.
            slow = slow->next;
            pos = pos->next;
        }
        return pos;
        */
    }
};