#ifndef CODE_GENERALALGORTIHMS_H
#define CODE_GENERALALGORTIHMS_H

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

template <class Type>
vector<pair<Type, unsigned long>> frequency_compress(vector<Type> list) {
    vector<pair<Type, unsigned long>> result;
    pair<Type, unsigned long> current = {list[0], 1};
    for (int i = 1; i < list.size(); i++) {
        if (list[i] == current.first)
            current.second++;
        else {
            result.push_back(current);
            current = {list[i], 1};
        }
    }
    result.push_back(current);
    return result;
}

/**
 * Merge Sort
 * ==========
 */

long long __inversions = 0;
void __merge(int *A, int start, int mid, int end) {
    int result[end - start];
    for (int x = start, y = mid; x < mid || y < end;) {
        if (x < mid && (y >= end || A[x] <= A[y])) {
            result[x + y - start - mid] = A[x];
            x++;
        } else {
            result[x + y - start - mid] = A[y];
            y++;
            __inversions += mid - x;
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
    __merge(A, start, (start + end) / 2, end);
}

/**
 * Multiplicative Function Sieve
 * =============================
 */

typedef long long ll;
const ll SIZE = 1e5 + 1;
ll phi[SIZE];
vector<ll> primes;
ll mobius[SIZE];
ll _lowest_prime_factor[SIZE];
ll _prime_factor_count[SIZE];

void construct_mobius() {
    for (ll i = 2; i < SIZE; i++) {
        _lowest_prime_factor[i] = i;
    }
    phi[0] = 0;
    phi[1] = 1;
    mobius[1] = 1;
    for (ll i = 2; i < SIZE; i++) {
        if (_lowest_prime_factor[i] == i) {
            mobius[i] = -1;
            phi[i] = i - 1;
            primes.push_back(i);
            _prime_factor_count[i] = 1;
        }
        for (auto p : primes) {
            ll ith_multiple = i * p;
            if (ith_multiple >= SIZE)
                break;
            _lowest_prime_factor[ith_multiple] =
                min(_lowest_prime_factor[i], p);
            if (i % p) {
                phi[ith_multiple] = phi[i] * phi[p];
                _prime_factor_count[ith_multiple] = _prime_factor_count[i] + 1;
                if (mobius[i])
                    mobius[ith_multiple] =
                        (_prime_factor_count[ith_multiple] & 1) ? -1 : 1;
                else
                    mobius[ith_multiple] = 0;
            } else {
                _prime_factor_count[ith_multiple] = _prime_factor_count[i];
                phi[ith_multiple] = phi[i] * p;
                mobius[ith_multiple] = 0;
                break;
            }
        }
    }
}

#endif  // CODE_GENERALALGORTIHMS_H