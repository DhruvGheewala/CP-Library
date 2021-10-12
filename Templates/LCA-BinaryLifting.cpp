class LCA {
private:
    int n, r;
    vector<int> lvl, dist;
    vector<vector<int>> par;
    graph g;

    void dfs(int u, int p) {
        if(par[u][0] != -1) return;  // visited

        par[u][0] = p;
        for(int i = 1; i < 20; i++)
            par[u][i] = par[par[u][i - 1]][i - 1];

        for(const auto &[v, w]: g[u]) {
            if(v != p) {
                dist[v] = dist[u] + w;
                lvl[v] = lvl[u] + 1;
                dfs(v, u);
            }
        }
    }

public:
    // constructor
    LCA(const graph &a, int root = 1) {
        g = a;
        n = g.size();

        lvl.assign(n, 0);
        dist.assign(n, 0);
        par.assign(n, vector<int>(20, -1));
        par[0] = vector<int>(20, 0);

        r = root;
        dfs(r, 0);
    }

    // methods
    int lca(int u, int v) {
        if(lvl[u] < lvl[v]) swap(u, v);
        int k = lvl[u] - lvl[v];
        for(int i = 0; k > 0; i++) {
            if(k & 1) u = par[u][i];
            k >>= 1;
        }

        if(u == v) return u;
        for(int i = 19; i >= 0; i--) {
            if(par[u][i] != par[v][i]) {
                u = par[u][i];
                v = par[v][i];
            }
        }
        assert(par[u][0] == par[v][0]);
        return par[u][0];
    }

    int root() { return r; }
    int distance(int u, int v) {return dist[u] + dist[v] - 2 * dist[lca(u, v)];}
};