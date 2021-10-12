// back[0] = 0;
// lvl = vector<int>(n, inf);

void dfs(int u, int p = 0) {
    if(lvl[u] < inf) {
        back[p] = min(back[p], lvl[u]);
        return;
    }

    lvl[u] = lvl[p] + 1;
    back[u] = lvl[u];

    bool isAp = true;
    for(const auto &[v, w]: adj[u]) {
        if(v == p) continue;

        dfs(v, u);
        back[u] = min(back[u], back[v]);

        isAp &= (back[u] <= back[v]);
        if(back[u] <= back[v]) {
            // Bridge: [u - v]
        }
    }
}