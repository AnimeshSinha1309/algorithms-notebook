#ifndef CODE_NUMBERTHEORY_H
#define CODE_NUMBERTHEORY_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

/**
 * Modular Arithmetic functions
 * ============================
 */

long long modPower(long long a, long long b, long long MOD) {
    long long cumulative = a, result = 1;
    for (; b > 0; b /= 2) {
        if (b % 2 == 1)
            result = (result * a) % MOD;
        cumulative = (cumulative * cumulative) % MOD;
    }
    return result;
}

long long modMultiply(long long a, long long b, long long MOD) {
    long long cumulative = a, result = 0;
    for (; b > 0; b /= 2) {
        if (b % 2 == 1)
            result = (result + a) % MOD;
        cumulative = (cumulative + cumulative) % MOD;
    }
    return result;
}

long long modInverse(long long a, long long MOD) {
    return modPower(a, MOD - 2, MOD);
}

/**
 * GCD and Prime Factorization
 * ===========================
 */

long long gcd(long long a, long long b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

vector<long long> prime_factorize(long long x) {
    vector<long long> result;
    if (x < 0) {
        x = -x;
        result.push_back(-1);
    }
    for (int i = 2; i <= sqrt(x); i++) {
        while (x % i == 0) {
            result.push_back(i);
            x /= i;
        }
    }
    if (x > 1)
        result.push_back(x);
    return result;
}

/**
 * Multiplicative Function Sieve
 * =============================
 */

typedef long long ll;

class Multiplicative {
// This is the definition for PHI
#define fn_prime_values(prime) (prime - 1)
#define fn_non_coprime(num, prime) (fn[num] * prime)
   public:
    ll size;
    vector<ll> fn;
    vector<ll> _primes;
    vector<ll> _lowest_prime_factor;

    Multiplicative(ll size) {
        size = size;
        _lowest_prime_factor = vector<ll>(size, 0);
        fn = vector<ll>(size, 0);
        // https://stackoverflow.com/questions/34260399
        // linear sieve
        for (ll i = 2; i < size; i++)
            _lowest_prime_factor[i] = i;
        // put any specific initialization code here like
        // multiplicativeFn[1] = 1;
        for (ll i = 2; i < size; i++) {
            if (_lowest_prime_factor[i] == i) {
                fn[i] = fn_prime_values(i);
                _primes.push_back(i);
            }
            for (auto p : _primes) {
                ll ith_multiple = i * p;
                if (ith_multiple >= size)
                    break;
                _lowest_prime_factor[ith_multiple] =
                    min(_lowest_prime_factor[i], p);
                if (i % p) {
                    fn[ith_multiple] = fn[i] * fn[p];
                } else {
                    fn[ith_multiple] = fn_non_coprime(i, p);
                    break;
                }
            }
        }
    }
};

#endif  // CODE_NUMBERTHEORY_H