// PROBLEM: http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_B

#include "../code/graphs.hpp"

int main() {
    // Prepare the Graph
    int n, m;
    cin >> n >> m;
    mii g(n);
    for (int i = 0; i < m; i++) {
        ll u, v, w;
        cin >> u >> v;
        g[u].push_back(v);
    }
    // Get the result from a call to Bellman Ford
    auto sorted = topological_sort(g);
    // Print the outputs
    for (int i = 0; i < n; i++) {
        cout << sorted[i] << endl;
    }
}