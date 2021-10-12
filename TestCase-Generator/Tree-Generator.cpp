void Tree(int n, vector<vi> &tree)
{
    vi code(n - 2);
    for(int i = 0 ; i < n - 2 ; i++)
        code[i] = rnd(1LL, n);

    vi deg(n + 1, 1);
    for(int u : code)
        deg[u]++;

    int ptr = 1;
    while(deg[ptr] != 1)
        ptr++;

    int leaf = ptr;
    for(int u : code)
    {
        tree[u].emplace_back(leaf);
        deg[u]--;
        if(deg[u] == 1 && u < ptr)
        {
            leaf = u;
            continue;
        }
        
        ptr++;
        while(deg[ptr] != 1)
            ptr++;
        leaf = ptr;
    }
    tree[n].emplace_back(leaf);
}