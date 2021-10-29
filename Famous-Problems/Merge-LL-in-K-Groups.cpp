// Variant-1: https://leetcode.com/problems/reverse-nodes-in-k-group/

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
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode *root, *ptr;
        root = ptr = new ListNode();
        
        ListNode *node = head;
        stack<ListNode *> st;
        while(node) {
            for(int i = 0; i < k && node; i++) {
                st.push(node);
                node = node -> next;
            }
            
            ListNode *begin = nullptr, *end = st.top();
            while(!st.empty() && st.size() < k) {
                begin = st.top();
                st.pop();
            }
            
            if(st.empty()) {
                ptr -> next = begin;
                ptr = end;
            }
            
            while(!st.empty()) {
                ptr -> next = st.top();
                st.pop();
                
                ptr = ptr -> next;
            }
        }
        ptr -> next = nullptr;
        return root -> next;
    }
};


// Variant-1: https://binarysearch.com/problems/Reverse-Linked-List-Groups

/**
 * class LLNode {
 *     public:
 *         int val;
 *         LLNode *next;
 * };
 */

LLNode* solve(LLNode* node, int k) {
    LLNode *root, *ptr;
    root = ptr = new LLNode();
    
    stack<LLNode *> st;
    while(node) {
        for(int i = 0; i < k && node; i++) {
            st.push(node);
            node = node -> next;
        }
        
        while(!st.empty()) {
            ptr -> next = st.top();
            st.pop();
            
            ptr = ptr -> next;
        }
    }
    ptr -> next = nullptr;
    return root -> next;
}