// Link: https://leetcode.com/problems/lfu-cache/

/**
 * Your LFUCache object will be instantiated and called as such:
 * LFUCache* obj = new LFUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

class LFUCache {
    struct DLL {
        int key, value, freq;
        DLL *prev, *next;
        
        DLL(int key = -1, int value = -1, int freq = 0) {
            this -> key = key;
            this -> value = value;
            this -> freq = freq;
            
            this -> prev = this -> next = nullptr;
        }
    };
    
    int capacity;
    DLL *head, *tail;
    unordered_map<int, DLL *> cache, freqHead;
    
    void insertBefore(DLL *node, DLL *from) {
        from -> prev -> next = node;
        node -> prev = from -> prev;
        
        from -> prev = node;
        node -> next = from;
    }
    
    void deleteNode(DLL *node) {
        node -> prev -> next = node -> next;
        node -> next -> prev = node -> prev;
        // free(node);
    }
    
public:
    LFUCache(int capacity) {
        head = new DLL();
        tail = new DLL();
        head -> next = tail;
        tail -> prev = head;
        
        cache.clear();
        freqHead.clear();
        this -> capacity = capacity;
    }
    
    int get(int key) {
        if(!cache.count(key) || capacity == 0)
            return -1;
        
        DLL *node = cache[key];
        int freq = node -> freq;
        
        if(node != freqHead[freq]) {
            DLL *ptr = (freqHead.count(freq + 1) ? freqHead[freq + 1] : freqHead[freq]);
            deleteNode(node);
            insertBefore(node, ptr);
        } else {
            if(node -> next -> freq == freq) freqHead[freq] = node -> next;
            else freqHead.erase(freq);
            
            if(freqHead.count(freq + 1)) {
                deleteNode(node);
                insertBefore(node, freqHead[freq + 1]);
            }
        }
        
        freq = freq + 1;
        node -> freq = freq;
        freqHead[freq] = node;
        return node -> value;
    }
    
    void put(int key, int value) {
        if(capacity == 0) return;
        
        if(!cache.count(key) && cache.size() == capacity) {
            DLL *node = tail -> prev;
            int freq = node -> freq;
            if(freqHead[freq] == node)
                freqHead.erase(freq);
            cache.erase(node -> key);
            deleteNode(node);
        }
        
        if(cache.count(key)) {
            get(key);
            DLL *node = cache[key];
            node -> value = value;
            return;
        }
        
        DLL *node = new DLL(key, value, 1);
        if(freqHead.count(1))
            insertBefore(node, freqHead[1]);
        else
            insertBefore(node, tail);
        
        cache[key] = node;
        freqHead[1] = node;
    }
};