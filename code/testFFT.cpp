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
const ll MOD = 1e9 + 7;

using cd = complex<double>;
const double PI = acos(-1);

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

template <typename T, typename A>
void pa(vector<T, A> const &a, ll begin = 0, ll end = -1) {
    if (end == -1)
        end = sz(a) - 1;
    for (ll i = begin; i <= end; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}

void solve() {
    vector<cd> pol1 = {1, 1}, pol2 = {1, 1};

    ll o1 = sz(pol1), o2 = sz(pol2), o3 = o1 + o2;
    o3 = 1ll << (ll)ceil(log2(o3));
    pol1.resize(o3);
    pol2.resize(o3);

    fft(pol1);
    fft(pol2);

    vector<cd> pol3(o3);
    for (ll i = 0; i < o3; i++) {
        pol3[i] = pol2[i] * pol1[i];
    }

    fft(pol3, true);

    pa(pol3);
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