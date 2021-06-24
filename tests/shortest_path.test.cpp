// PROBLEM: https://judge.yosupo.jp/problem/shortest_path
#include "../code/graphs.hpp"

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vpl> adj(n);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].emplace_back(v, w);
    }
    auto ans = dijkstra(adj, s);
    vll dist = ans.first, parents = ans.second;
    if (dist[t] == INT64_MAX) {
        cout << -1 << endl;
    } else {
        vector<int> path = {t};
        while (path.back() != s) {
            path.push_back(parents[path.back()]);
        }
        cout << dist[t] << ' ' << path.size() - 1 << endl;
        for (int i = path.size() - 1; i > 0; i--) {
            cout << path[i] << ' ' << path[i - 1] << endl;
        }
    }
}