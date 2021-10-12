class Fenwick {
private:
    int n;
    vector<int> tree;

public:
    Fenwick() {}
    
    Fenwick(int N) {
        n = N + 1;
        tree.assign(n, 0);
    }

    void update(int idx, int val) {
        for(; idx < n; idx += (idx & -idx))
            tree[idx] = val;
    }

    int prefix(int idx) {
        int result = 0;
        for(; idx > 0; idx -= (idx & -idx))
            result += tree[idx];
        return result;
    }

    int sum(int l, int r) {return prefix(r) - prefix(l - 1);}
    int suffix(int idx) {return prefix(n - 1) - prefix(idx - 1);}
};