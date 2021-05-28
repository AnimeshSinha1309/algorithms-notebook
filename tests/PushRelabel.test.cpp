#define TURL "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A"

#include "../code/FlowAlgorithms.hpp"

#include <iostream>
using namespace std;

int main() {
    // Prepare the Graph
    int n, m;
    cin >> n >> m;
    PushRelabel g(n);
    for (int i = 0; i < m; i++) {
        ll u, v, w;
        cin >> u >> v >> w;
        g.add_edge(u, v, w);
    }
    // Get the result from a call to Dijkstra
    ll res = g.max_flow(0, n - 1);
    // Print the outputs
    cout << res << endl;
    return 0;
}