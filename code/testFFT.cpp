#include <iostream>
#include <cmath>
#include <complex>
#include <vector>
#ifdef ONLINE_JUDGE
#define endl "\n"
#endif
using namespace std;
typedef long long int ll;
#define sz(x) (ll) x.size()
const ll MOD = 998244353;

using cd = complex<double>;
const double PI = acos(-1);

ll inverse(ll x, ll mod) {
    ll res = 1;
    x = x % mod;
    ll y = mod - 2;

    while (y > 0) {
        if (y & 1)
            res = (res * x) % mod;
        y = y >> 1;
        x = (x * x) % mod;
    }

    return res;
}

void fft(vector<cd> &a, bool invert = false) {
    int n = (int)a.size();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd &x : a)
            x /= n;
    }
}

void solve() {
    cout << inverse(565042129, MOD);
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen("file", "r", stdin);
    // freopen("file", "w", stdout);
    // ll t;cin >> t;while(t--)
    solve();
    return 0;
}