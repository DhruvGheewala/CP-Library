// Link: https://leetcode.com/problems/copy-list-with-random-pointer/

/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};
*/

class Solution {
public:
    Node* copyRandomList(Node* head) {
    	// Intution of the efficient approch
        // map<Node *, Node *> mp;
        // Node *ptr = head;
        
        // while(ptr) {
        //     mp[ptr] = new Node(ptr -> val);
        //     ptr = ptr -> next;
        // }
        
        // ptr = head;
        // while(ptr) {
        //     Node *node = mp[ptr];
        //     node -> next = mp[ptr -> next];
        //     node -> random = mp[ptr -> random];
        //     ptr = ptr -> next;
        // }
        // return mp[head];

        Node *ptr = head; // doing same as `mp[ptr] = new Node(ptr -> val);`
        while(ptr) {
            Node *next = ptr -> next;
            ptr -> next = new Node(ptr -> val);
            ptr -> next -> next = next;
            ptr = next;
        }
        
        ptr = head; // mapping random pointers
        while(ptr) {
            Node *next = ptr -> next;
            Node *random_next = ptr -> random ? ptr -> random -> next : nullptr;
            next -> random = random_next;
            ptr = next -> next;
        }
        
        ptr = head;
        Node *root = new Node(-1);
        Node *node = root;
        // separating two Lists
        while(ptr) {
            Node *next = ptr -> next;
            node -> next = next;
            ptr -> next = next -> next;
            node = next;
            ptr = ptr -> next;
        }
        return root -> next;
    }
};