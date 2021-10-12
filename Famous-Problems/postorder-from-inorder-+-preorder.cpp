vector<int> postOrder(vector<int> preorder, vector<int> inorder) {
    int n = preorder.size();
    assert(inorder.size() == n);

    vector<int> res;
    res.reserve(n);

    map<int, int> mp;
    for(int i = 0; i < n; i++)
        mp[inorder[i]] = i;

    int index = 0;
    function<void(int, int)> rec = [&](int l, int r) {
        if(l > r) return;

        assert(mp.count(preorder[index]));
        int m = mp[preorder[index]];
        index++;

        rec(l, m - 1);
        rec(m + 1, r);
        res.emplace_back(inorder[m]);
    };

    rec(0, n - 1);
    return res;
}