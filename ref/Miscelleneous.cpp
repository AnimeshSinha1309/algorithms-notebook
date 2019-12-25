#include "template.hpp"

alias gww='g++ -Wall -pedantic -pedantic-errors -Wextra -Wcast-align -Wcast-qual -Wconversion -Wfloat-equal -Wformat=2 -Wformat-nonliteral -Winit-self -Winline -Winvalid-pch -Wmissing-field-initializers -Wmissing-noreturn -Wpointer-arith -Wredundant-decls -Wstack-protector -Wstrict-aliasing=2 -Wswitch-default -Wunreachable-code -Wunused -Wunused-but-set-variable -Wunused-parameter'
alias gmm="gww -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG"

#pragma GCC optimize("Ofast")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

ll gcd(ll a, ll b, ll &x, ll &y) {int g = a;x = 1, y = 0;if (b != 0)g = gcd(b, a % b, y, x), y -= a / b * x;return g;}
ll mod_inverse(ll a, ll mod) {ll x, y;gcd(a, mod, x, y);return (x + mod) % mod;}
// find z: z % m1 = r1, z % m2 = r2. Here, z is unique modulo M = lcm(m1, m2).
// Return (z, M). On failure, M = -1.
pll CRT(int m1, int r1, int m2, int r2) {int s, t;int g = extended_euclid(m1, m2, s, t);
// m1s+m2t=g
if (r1 % g != r2 % g)return {0, -1};return {mod(s * r2 * m1 + t * r1 * m2, m1 * m2) / g, m1 * m2 / g};}

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag,
             tree_order_statistics_node_update>
    ordered_set;

// deterministic for all n <=2 ^ 64
bool MRPrime(ll N) {int primes[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};if (N <= 1)return false;for (auto p : primes) {if (p == N)return true;if (N % p == 0)return false;}ll c = N - 1, g = 0;while (!(c & 1))c >>= 1, ++g;for (auto p : primes) {ll k = mod_power(p, c, N);for (int j = 0; j < g; ++j) {ll kk = mod_multiply(k, k, N);if (kk == 1 && k != 1 && k != N - 1)return false;k = kk;}if (k != 1)return false;}return true;}