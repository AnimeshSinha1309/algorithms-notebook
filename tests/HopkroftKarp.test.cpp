#define TURL "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_7_A"

#include "../code/FlowAlgorithms.hpp"

#include <iostream>
using namespace std;

int main() {
    // Prepare the Graph
    int n_x, n_y, m;
    cin >> n_x >> n_y >> m;
    HopcroftKarp g(n_x, n_y);
    for (int i = 0; i < m; i++) {
        ll u, v;
        cin >> u >> v;
        g.add_edge(u, v);
    }
    // Get the result from a call to Dijkstra
    ll res = g.max_match();
    // Print the outputs
    cout << res << endl;
    return 0;
}