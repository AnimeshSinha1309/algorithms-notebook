#define TURL "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A"

#include "../code/Graphs.hpp"

#include <iostream>
using namespace std;

int main() {
    // Prepare the Graph
    int n, m, s;
    cin >> n >> m >> s;
    Graph g(n);
    for (int i = 0; i < m; i++) {
        ll u, v, w;
        cin >> u >> v >> w;
        g.add_edge(u, v, w);
    }
    // Get the result from a call to Dijkstra
    pair<vll, vll> res = g.dijkstra({s});
    // Print the outputs
    for (int i = 0; i < n; i++) {
        if (res.first[i] == INT64_MAX)
            cout << "INF" << endl;
        else
            cout << res.first[i] << endl;
    }
    return 0;
}
