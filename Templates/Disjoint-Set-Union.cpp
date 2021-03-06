/****
  ** Time Complexity : O(nlogn)
  ** The time complexity is good because for each tool, 
  ** we take and put it in an other box at most log(n) times
  ** (each time the size of the vector will be at least doubled).
  **
  ** Space Complexity : O(n)
****/

class DSU {
private:
    // Variables
    vector<int> par;
    int boxes, tools;

    // Methods
    inline bool sameBox(int u, int v) {return find(u) == find(v);}

public:
    // Constructors
    DSU(int size) {build(size);}

    // Methods
    inline void build(int size) {
        clear();

        tools = size;
        boxes = size;

        // par[i] < 0 => i is root
        // iff i is root => -par[i] is sizeof that box
        par.assign(tools + 1, -1);
    }

    inline int size() {return tools + 1;}       // 0...tools
    inline int disjoints() {return boxes + 1;}  // 0...boxes
    
    inline int size(int u) {return -par[find(u)];}
    inline int find(int u) {return par[u] < 0 ? u : par[u] = find(par[u]);}

    inline bool merge(int u, int v, bool choice = false) {
        if(sameBox(u, v)) return false;

        u = find(u);
        v = find(v);
        if(!choice && size(u) < size(v)) swap(u, v);

        par[u] += par[v];  // inc the size of new root u
        par[v] = u;  // now root of whole box v is u
        boxes--;
        return true;
    }

    inline void clear() {
        boxes = 0;
        tools = 0;
        par.clear();
    }

    // For Debugging
    inline int operator [](int i) {
        assert(i <= tools);
        return par[i];
    }
};

string to_string(DSU a)
{
    #ifdef DEBUG
        string res;
        int n = a.size();
        
        vector<int> par(n);
        for(int i = 0; i < n; i++) {
            par[i] = a.find(i);
        }
        res = "Parent: " + to_string(par);
        
        return res;
    #endif
    return "";
}