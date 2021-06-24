// PROBLEM: https://judge.yosupo.jp/problem/bitwise_and_convolution
#include "../code/convolutions.hpp"

int main() {
    const ll MOD = 998244353;
    int n;
    cin >> n;
    n = 1 << n;
    vector<ll> x(n), y(n);
    for (int i = 0; i < n; i++)
        cin >> x[i];
    for (int i = 0; i < n; i++)
        cin >> y[i];

    zeta_transform(x);
    zeta_transform(y);
    for (int i = 0; i < n; i++) {
        x[i] = (x[i] * y[i]) % MOD;
    }
    mobius_transform(x);
    for (int i = 0; i < n; i++)
        cout << x[i] % MOD << ' ';
    cout << endl;
}
