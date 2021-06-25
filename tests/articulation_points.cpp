// PROBLEM: http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_A

#include "../code/connectivity.hpp"

int main() {
    int n, m;
    cin >> n >> m;
    mii g(n);
    for (int i = 0; i < m; i++) {
        ll u, v, w;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    auto points = articulation_points(g);
    for (auto el : points) {
        cout << el << endl;
    }
}