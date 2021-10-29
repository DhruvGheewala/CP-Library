// Link: https://leetcode.com/problems/reverse-linked-list-ii/

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
private:
	pair<ListNode *, ListNode *> reverse(ListNode *head, int len) {
		ListNode *prev = nullptr;
		ListNode *current = head;
		
		ListNode *next, *begin, *end;
		end = head;

		while(current && len > 0) {
			next = current -> next;
			current -> next = prev;
			prev = current;
			current = next;
			len = len - 1;
		}

		begin = prev;
		end -> next = current;
		return {begin, end};
	}

public:
	ListNode *reverseBetween(ListNode *head, int left, int right) {
		ListNode *root = new ListNode();
		ListNode *ptr = root;
		root -> next = head;

		ListNode *prev = ptr;
		ptr = ptr -> next;

		for(int i = 1; ptr; i++) {
			if(i == left) {
				auto [begin, end] = reverse(ptr, right - left + 1);
				prev -> next = begin;
				ptr = end;
				i = right;
			}
            prev = ptr;
			ptr = ptr -> next;
		}
		return root -> next;
	}
};