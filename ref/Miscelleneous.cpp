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

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;