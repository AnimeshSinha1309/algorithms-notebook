ll binary_search(ll TOP, ll BOT, function<bool(ll)> check) {
    ll result = 0;
    for (ll top = 1e5, bot = 0, mid = bot + (top - bot) / 2; bot <= top;
         mid = bot + (top - bot) / 2) {
        if (check(mid) && !check(mid - 1)) {
            result = mid;
            break;
        }
        (check(mid)) ? (top = mid - 1) : (bot = mid + 1);
    }
}

ll gcd(ll a, ll b, ll &x, ll &y) {
    int g = a;
    x = 1, y = 0;
    if (b)
        g = gcd(b, a % b, y, x), y -= a / b * x;
    return g;
}
ll mod_inverse(ll a, ll mod) {
    ll x, y;
    gcd(a, mod, x, y);
    return (x + mod) % mod;
}

long long _inv = 0;
void _merge(int A[], int start, int mid, int end) {
    int result[end - start];
    for (int x = start, y = mid; x < mid || y < end;) {
        if (x < mid && (y >= end || A[x] <= A[y])) {
            result[x + y - start - mid] = A[x];
            x++;
        } else {
            result[x + y - start - mid] = A[y];
            y++;
            _inv += mid - x;
        }
    }
    for (int i = start; i < end; i++)
        A[i] = result[i - start];
}
void sort(int A[], int start, int end) {
    if (start >= end - 1)
        return;
    sort(A, start, (start + end) / 2);
    sort(A, (start + end) / 2, end);
    _merge(A, start, (start + end) / 2, end);
}

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef ordered_set tree<int, null_type, less<int>, rb_tree_tag,
                         tree_order_statistics_node_update>;