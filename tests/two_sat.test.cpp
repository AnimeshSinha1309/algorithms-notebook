// PROBLEM: https://judge.yosupo.jp/problem/two_sat
#include "../code/scc.hpp"

int main() {
    int n, m;
    string dummy;
    cin >> dummy >> dummy >> n >> m;
    Satisfiability sat(n);
    for (int i = 0; i < m; i++) {
        int v1, v2;
        cin >> v1 >> v2 >> dummy;
        int x1 = v1 < 0 ? sat.NOT(sat.var(-v1 - 1)) : sat.var(v1 - 1);
        int x2 = v2 < 0 ? sat.NOT(sat.var(-v2 - 1)) : sat.var(v2 - 1);
        sat.add_or(x1, x2);
    }
    if (sat.solvable()) {
        cout << "s SATISFIABLE\nv ";
        auto vec = sat.solve();
        for (int i = 0; i < n; i++) {
            cout << (vec[i] ? (i + 1) : -(i + 1)) << ' ';
        }
        cout << "0 " << endl;
    } else {
        cout << "s UNSATISFIABLE" << endl;
    }
}
