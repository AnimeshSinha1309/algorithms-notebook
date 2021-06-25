#include "template.hpp"

set<int> articulation_points(mii adj) {
    int n = adj.size();
    vector<bool> visited;
    vector<int> t_start, t_low;
    set<int> cutpoints;
    int timer = 0;
    visited.assign(n, false), t_start.assign(n, -1), t_low.assign(n, -1);

    function<void(int, int)> dfs = [&](int v, int p) {
        visited[v] = true;
        t_start[v] = t_low[v] = timer++;
        int children = 0;
        for (int to : adj[v]) {
            if (to == p)
                continue;
            if (visited[to]) {
                t_low[v] = min(t_low[v], t_start[to]);
            } else {
                dfs(to, v);
                t_low[v] = min(t_low[v], t_low[to]);
                if (t_low[to] >= t_start[v] && p != -1)
                    cutpoints.insert(v);
                ++children;
            }
        }
        if (p == -1 && children > 1)
            cutpoints.insert(v);
    };

    for (int i = 0; i < n; ++i) {
        if (!visited[i])
            dfs(i, -1);
    }
    return cutpoints;
}
