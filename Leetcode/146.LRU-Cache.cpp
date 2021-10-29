// Link: https://leetcode.com/problems/lru-cache/

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

class LRUCache {
private:
    struct DLL {
        int key, value;
        DLL *prev, *next;

        DLL(int key = -1, int value = -1) {
            this -> key = key;
            this -> value = value;
            this -> prev = this -> next = nullptr;
        }
    };
    
    int capacity;
    DLL *head, *tail;
    unordered_map<int, DLL *> cache;

    void deleteNode(DLL *node) {
        node -> prev -> next = node -> next;
        node -> next -> prev = node -> prev;
    }
    
    void insertAfterHead(DLL *node) {
        head -> next -> prev = node;
        node -> next = head -> next;
        head -> next = node;
        node -> prev = head;
    }
    
public:
    LRUCache(int capacity) {
        head = new DLL();
        tail = new DLL();
        
        head -> next = tail;
        head -> prev = nullptr;
        
        tail -> next = nullptr;
        tail -> prev = head;
        
        cache.clear();
        this -> capacity = capacity;
    }
    
    int get(int key) {
        if(!cache.count(key))
            return -1;
        
        DLL *node = cache[key];
        deleteNode(node);
        insertAfterHead(node);
        return node -> value;
    }
    
    void put(int key, int value) {
        if(!cache.count(key)) {
            if(cache.size() == capacity) {
                cache.erase(tail -> prev -> key);
                deleteNode(tail -> prev);
            }
            
            DLL *node = new DLL(key, value);
            cache[key] = node;
            insertAfterHead(node);
        } else {
            DLL *node = cache[key];
            node -> value = value;
            deleteNode(node);
            insertAfterHead(node);
        }
    }
};