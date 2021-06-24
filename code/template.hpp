#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vii;
typedef vector<vii> mii;
typedef pair<int, int> pii;
typedef vector<pii> vpi;

typedef long long ll;
typedef vector<ll> vll;
typedef pair<ll, ll> pll;
typedef vector<vll> mll;
typedef vector<pll> vpl;

typedef long double ld;
typedef vector<long double> vld;
typedef vector<vector<long double>> mld;

typedef vector<bool> vbl;
typedef vector<vector<bool>> mbl;

template <typename Type>
istream &operator>>(istream &in, vector<Type> &vec) {
    ll n = vec.size();
    for (int i = 0; i < n; i++)
        in >> vec[i];
    return in;
}
template <typename Type>
ostream &operator<<(ostream &out, vector<Type> &vec) {
    for (auto val : vec)
        out << val << " ";
    out << endl;
    return out;
}

ll mod_power(ll a, ll b, ll MOD) {
    ll cumulative = a, result = 1;
    for (; b > 0; b /= 2) {
        if (b % 2 == 1)
            result = (result * cumulative) % MOD;
        cumulative = (cumulative * cumulative) % MOD;
    }
    return result;
}

ll mod_multiply(ll a, ll b, ll MOD) {
    ll cumulative = a, result = 0;
    for (; b > 0; b /= 2) {
        if (b % 2 == 1)
            result = (result + cumulative) % MOD;
        cumulative = (cumulative + cumulative) % MOD;
    }
    return result;
}

ll mod_inverse(ll a, ll MOD) {
    return mod_power(a, MOD - 2, MOD);
}
