// PROBLEM: https://judge.yosupo.jp/problem/convolution_mod
#include "../code/convolutions.hpp"

int main() {
    const int MOD = 998244353;
    int n, m;
    cin >> n >> m;
    vector<int> x(n), y(m);
    for (int i = 0; i < n; i++)
        cin >> x[i];
    for (int i = 0; i < m; i++)
        cin >> y[i];
    int order = 1;
    while (order < n + m - 1)
        order <<= 1;
    x.resize(order);
    y.resize(order);

    ntt(x);
    ntt(y);
    for (int i = 0; i < order; i++) {
        x[i] = (int)((1ll * x[i] * y[i]) % MOD);
    }
    ntt(x, true);
    for (int i = 0; i < n + m - 1; i++)
        cout << x[i] << ' ';
    cout << endl;
}
