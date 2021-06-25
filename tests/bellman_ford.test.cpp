// PROBLEM: "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_B"

#include "../code/graphs.hpp"

int main() {
    // Prepare the Graph
    int n, m, s;
    cin >> n >> m >> s;
    vector<vpl> g(n);
    for (int i = 0; i < m; i++) {
        ll u, v, w;
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
    }
    // Get the result from a call to Bellman Ford
    pair<vll, vll> res = bellman_ford(g, s);
    // Print the outputs
    if (*min_element(res.first.begin(), res.first.end()) == INT64_MIN) {
        cout << "NEGATIVE CYCLE" << endl;
        return 0;
    }
    for (int i = 0; i < n; i++) {
        if (res.first[i] == INT64_MAX)
            cout << "INF" << endl;
        else
            cout << res.first[i] << endl;
    }
}