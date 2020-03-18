#define PROBLEM http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_1_A

#include "DisjointSet.hpp"

#include <iostream>
using namespace std;

int main() {
    int n, q;
    cin >> n >> q;
    DisjointSetTree dsu(n);
    while (q--) {
        int com, x, y;
        cin >> com >> x >> y;
        if (com == 0) {
            dsu.merge(x, y);
        } else if (com == 1) {
            cout << (dsu.find(x) == dsu.find(y)) << endl;
        }
    }
    return 0;
}
