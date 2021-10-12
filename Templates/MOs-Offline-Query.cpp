// https://codeforces.com/contest/86/problem/D

/**** First Approch (apprx. 1200-1300 ms, standard MO) ****/
vi freq;
struct query {int l, r, id;};

void QueryMO(vi &arr, vector<query> &q)
{
    /**** Fixed ****/
    vi answer(q.size(), -1);
    int l, r, n, ans, bucket;
    
    n = arr.size();
    bucket = sqrt(n) + 0.5;
    auto cmp = [&](query &lhs, query &rhs) {
        int b1, b2;
        b1 = lhs.l / bucket;
        b2 = rhs.l / bucket;
        return b1 != b2 ? b1 < b2 : b1 & 1 ? lhs.r < rhs.r : rhs.r < lhs.r;
    };
    sort(all(q), cmp);
    /**** Fixed ****/

    /**** Operations ****/
    auto add = [&](int id, int &res) {
        int x = arr[id];
        res -= freq[x] * freq[x] * x;
        freq[x]++;
        res += freq[x] * freq[x] * x;
    };

    auto remove = [&](int id, int &res) {
        int x = arr[id];
        res -= freq[x] * freq[x] * x;
        freq[x]--;
        res += freq[x] * freq[x] * x;
    };
    /**** Operations ****/
    
    /**** Fixed ****/
    r = -1;
    l = ans = 0;
    for(int i = 0; i < q.size(); i++) {
        while(l < q[i].l) remove(l++, ans);
        while(l > q[i].l) add(--l, ans);
        while(r < q[i].r) add(++r, ans);
        while(r > q[i].r) remove(r--, ans);
        answer[q[i].id] = ans;
    }
    /**** Fixed ****/

    /**** Printing Answer ****/    
    for(int i = 0 ; i < q.size() ; i++)
        cout << answer[i] << endl;
}

/**** Second Approch(apprx. 700-800 ms, hilbert + MO) ****/
vi freq;
struct query {
    // Note : calc eval for every query
    int l, r, id, eval;
    
    inline void calcEval() {eval = giveOrder(l, r, 21, 0);}
    inline friend bool operator <(const query &lhs, const query &rhs) {return lhs.eval < rhs.eval;}

    inline int64_t giveOrder(int x, int y, int pow, int rotate)
    {
        if(pow == 0) return 0;
        int hpow = 1 << (pow - 1);
        int seg = x < hpow ? y < hpow ? 0 : 3 : y < hpow ? 1 : 2;
        seg = (seg + rotate) & 3;
        const static int rotateDelta[4] = {3, 0, 0, 1};
        int nx = x & (x ^ hpow);
        int ny = y & (y ^ hpow);
        int nrot = (rotate + rotateDelta[seg]) & 3;
        int64_t subSquareSize = 1LL << ((pow - 1) << 1);
        int64_t ans = seg * subSquareSize;
        int64_t add = giveOrder(nx, ny, pow - 1, nrot);
        ans += (seg == 1 || seg == 2) ? add : subSquareSize - add - 1;
        return ans;
    }
};

void QueryMO(vi &arr, vector<query> &q)
{
    /**** Fixed ****/
    vi answer(q.size(), -1);
    int l, r, n, ans, bucket;
    n = arr.size();
    sort(all(q));
    /**** Fixed ****/

    /**** Operations ****/
    auto add = [&](int id, int &res) {
        int x = arr[id];
        res -= freq[x] * freq[x] * x;
        freq[x]++;
        res += freq[x] * freq[x] * x;
    };

    auto remove = [&](int id, int &res) {
        int x = arr[id];
        res -= freq[x] * freq[x] * x;
        freq[x]--;
        res += freq[x] * freq[x] * x;
    };
    /**** Operations ****/
    
    /**** Fixed ****/
    r = -1;
    l = ans = 0;
    for(int i = 0; i < q.size(); i++) {
        while(l < q[i].l) remove(l++, ans);
        while(l > q[i].l) add(--l, ans);
        while(r < q[i].r) add(++r, ans);
        while(r > q[i].r) remove(r--, ans);
        answer[q[i].id] = ans;
    }
    /**** Fixed ****/

    /**** Printing Answer ****/    
    for(int i = 0 ; i < q.size() ; i++)
        cout << answer[i] << endl;
}