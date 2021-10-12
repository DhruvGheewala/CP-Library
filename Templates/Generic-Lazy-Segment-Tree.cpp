template<typename node, typename update>
class segtree {
private:
    // usefull variables
    int k;
    int32_t N, ql, qr, qi;
    update upd;

    // tree structure & lazy tree
    vector<node> tree;
    vector<update> updates;
    vector<bool> lazy;

    // identity elements
    const node identity = node();
    const update identity_transformation = update();

    void build(const vector<node> &data, const int32_t &p, const int32_t &l, const int32_t &r) {
        if(l == r) {
            tree[p] = data[l];
            return;
        }

        int32_t m = (l + r) >> 1;
        build(data, p << 1, l, m);
        build(data, p << 1 | 1, m + 1, r);
        tree[p].merge(tree[p << 1], tree[p << 1 | 1]);
    }

    void apply(const int32_t &p, const int32_t &l, const int32_t &r, update &u) {
        if(l != r) {
            lazy[p] = true;
            updates[p].combine(u, l, r);
        }
        u.apply(tree[p], l, r);
    }

    void pushdown(const int32_t &p, const int32_t &l, const int32_t &r) {
        if(!lazy[p]) return;
        
        int32_t m = (l + r) >> 1;
        apply(p << 1, l, m, updates[p]);
        apply(p << 1 | 1, m + 1, r, updates[p]);
        updates[p] = identity_transformation;
        lazy[p] = false;
    }

    // range query => merge([ql, qr])
    node answer(const int32_t &p, const int32_t &l, const int32_t &r) {
        if(ql > r || qr < l) {
            return identity;
        }
        if(l >= ql && r <= qr) {
            return tree[p];
        }

        pushdown(p, l, r);
        int32_t m = (l + r) >> 1;
        node left = answer(p << 1, l, m);
        node right = answer(p << 1 | 1, m + 1, r);
        
        node ans;
        ans.merge(left, right);
        return ans;
    }

    // range update => [ql, qr].apply(upd)
    void rupd(const int32_t &p, const int32_t &l, const int32_t &r) {
        if(ql > r || qr < l) {
            return;
        }
        if(ql <= l && r <= qr) {
            apply(p, l, r, upd);
            return;
        }

        pushdown(p, l, r);
        int32_t m = (l + r) >> 1;
        rupd(p << 1, l, m);
        rupd(p << 1 | 1, m + 1, r);
        tree[p].merge(tree[p << 1], tree[p << 1 | 1]);
    }

    // returns greatest i such that, k <= a[i] & i <= qi
    int32_t descentLeft(int32_t p, int32_t l, int32_t r, node &right) {
        if(qi < l) return -1;
        if(l <= qi) {
            node cur;
            cur.merge(tree[p], right);
            if(!cur.check(k)) {
                right = cur;
                return -1;
            }
            if(l == r) return l;
        }

        pushdown(p, l, r);
        int32_t m = (l + r) >> 1;
        int32_t res = descentLeft(p << 1 | 1, m + 1, r, right);
        if(res >= 0) return res;
        return descentLeft(p << 1, l, m + 1, right);
    }

    // returns smallest i such that, k <= a[i] & qi <= i
    int32_t descentRight(int32_t p, int32_t l, int32_t r, node &left) {
        if(r < qi) return size();
        if(qi <= l) {
            node cur;
            cur.merge(left, tree[p]);
            if(!cur.check(k)) {
                left = cur;
                return size();
            }
            if(l == r) return l;
        }

        pushdown(p, l, r);
        int32_t m = (l + r) >> 1;
        int32_t res = descentRight(p << 1, l, m, left);
        if(res < size()) return res;
        return descentRight(p << 1 | 1, m + 1, r, left);
    }

public:
    segtree(int32_t n = 0) {
        vector<node> data(n);
        build(data);
    }

    segtree(const vector<node> &data) {
        build(data);
    }

    void build(const vector<node> &data) {
        int n = data.size();
        N = n << 2;

        tree.resize(N);
        updates.resize(N);
        lazy.assign(N, false);

        build(data, 1, 0, n - 1);
    }

    node answer(const int32_t &l, const int32_t &r) {
        assert(l <= r);
        ql = l;
        qr = r;
        return answer(1, 0, size() - 1);
    }

    void rupd(const int32_t &l, const int32_t &r, const update &_u) {
        assert(l <= r);
        ql = l;
        qr = r;
        upd = _u;
        rupd(1, 0, size() - 1);
    }

    int32_t descentLeft(int32_t i, int _k) {
        qi = i;
        k = _k;
        node right = identity;
        return descentLeft(1, 0, size() - 1, right);
    }

    int32_t descentRight(int32_t i, int _k) {
        qi = i;
        k = _k;
        node left = identity;
        return descentRight(1, 0, size() - 1, left);
    }
    
    void clear() {
        tree.clear();
        lazy.clear();
        updates.clear();
    }

    vector<bool> getLazy() {return lazy;}
    int32_t size() {return N >> 2;}
};

template<typename T, typename U>
string to_string(segtree<T, U> a)
{
    #ifdef DEBUG
        string res;
        int n = a.size();
        res = "{\n";
        res += "\tsize: " + to_string(n) + "\n";
        res += "\tLazy nodes: " + to_string(a.getLazy()) + "\n";
        for(int i = 0; i < n; i++) {
            bool first = true;
            for(int j = i; j < n; j++) {
                res += (first ? "\t" : ", ");
                res += "[" + to_string(i);
                res += ", " + to_string(j);
                res += "]: ";
                res += to_string(a.answer(i, j));
                first = false;
            }
            res += "\n";
        }
        res += "}";
        return res;
    #endif
    return "";
}

// node.merge(val, identity) = node.merge(identity, val) = val
struct segData {
    int v;

    // identity
    segData(int _v = 0) {
        v = _v;
    }

    // merging function
    void merge(const segData &lhs, const segData &rhs) {
        v = lhs.v + rhs.v;
    }

    bool check(int k) {
        return k <= v;
    }
};

string to_string(const segData &a)
{
    #ifdef DEBUG
        string res;
        res = "<" + to_string(a.v) + ">";
        return res;
    #endif
    return "";
}

// identity_transformation.combine(upd) = upd
struct lazyUpdate {
    int v;

    // identity_transformation
    lazyUpdate(int _v = 0) {
        v = _v;
    }

    // combine the new update with old update
    void combine(const lazyUpdate &new_update, const int32_t &l, const int32_t &r) {
        v += new_update.v;
    }

    // apply lazy value to node x
    void apply(segData &x, const int32_t &l, const int32_t &r) {
        x.v += v;
    }
};

string to_string(const lazyUpdate &a)
{
    #ifdef DEBUG
        string res;
        res = "<" + to_string(a.v) + ">";
        return res;
    #endif
    return "";
}

template<typename node = segData, typename update = lazyUpdate>
using SegmentTree = segtree<node, update>;