// PROBLEM: https://judge.yosupo.jp/problem/bitwise_xor_convolution
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

    walsh_hadamard_transform(x);
    walsh_hadamard_transform(y);
    for (int i = 0; i < n; i++) {
        x[i] = (x[i] * y[i]) % MOD;
    }
    walsh_hadamard_transform(x);
    const ll inv = (2 * MOD - ((MOD - 1) * mod_inverse(n, MOD)) % MOD) % MOD;
    for (int i = 0; i < n; i++)
        cout << (x[i] * inv) % MOD << ' ';
    cout << endl;
}
