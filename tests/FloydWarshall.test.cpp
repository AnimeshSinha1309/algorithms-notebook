#define TURL "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_C"

#include "../code/Graphs.hpp"

#include <iostream>
using namespace std;

int main() {
    // Prepare the Graph
    int n, m;
    cin >> n >> m;
    Graph g(n);
    for (int i = 0; i < m; i++) {
        ll u, v, w;
        cin >> u >> v >> w;
        g.add_edge(u, v, w, false);
    }
    // Get the result from a call to Floyd Warshall
    mll res = g.floyd_warshall();
    // Print the outputs
    for (int i = 0; i < n; i++) {
        if (res[i][i] < 0) {
            cout << "NEGATIVE CYCLE" << endl;
            return 0;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (res[i][j] == INT64_MAX)
                cout << "INF";
            else
                cout << res[i][j];
            if (j != n - 1)
                cout << " ";
        }
        cout << endl;
    }
    return 0;
}
