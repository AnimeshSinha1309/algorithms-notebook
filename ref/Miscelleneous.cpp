#include "template.hpp"

ll binary_search(ll TOP, ll BOT, function<bool(ll)> check) {
    ll result = BOT - 1;
    for (ll top = TOP, bot = BOT, mid = bot + (top - bot) / 2; bot <= top; mid = bot + (top - bot) / 2) {
        if (check(mid) && !check(mid - 1)) { result = mid; break; }
        (check(mid)) ? (top = mid - 1) : (bot = mid + 1);
    }
}

ll gcd(ll a, ll b, ll &x, ll &y) {
    int g = a; x = 1, y = 0;
    if (b != 0) g = gcd(b, a % b, y, x), y -= a / b * x;
    return g;
}
ll mod_inverse(ll a, ll mod) {
    ll x, y; gcd(a, mod, x, y);
    return (x + mod) % mod;
}

ll mod_power(ll a, ll b, ll MOD) {
    ll cumulative = a, result = 1;
    for (; b > 0; b /= 2) {
        if (b % 2 == 1) result = (result * cumulative) % MOD;
        cumulative = (cumulative * cumulative) % MOD;
    } return result;
}
ll mod_multiply(ll a, ll b, ll MOD) {
    ll cumulative = a, result = 0;
    for (; b > 0; b /= 2) {
        if (b % 2 == 1) result = (result + cumulative) % MOD;
        cumulative = (cumulative + cumulative) % MOD;
    } return result;
}

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

// deterministic for all n <=2 ^ 64
bool MRPrime(ll N) { 
    int primes[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}; if (N <= 1)return false;
    for (auto p : primes) { if (p == N)return true; if (N % p == 0)return false; }
    ll c = N - 1, g = 0; while (!(c & 1)) c >>= 1, ++g; 
    for (auto p : primes) { 
        ll k = mod_power(p, c, N); 
        for (int j = 0; j < g; ++j) { ll kk = mod_multiply(k, k, N); if (kk == 1 && k != 1 && k != N - 1) return false; k = kk; }
        if (k != 1) return false;
    } return true;
}