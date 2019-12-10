#ifndef CODE_FASTFOURIER_H
#define CODE_FASTFOURIER_H

#include <cmath>
#include <complex>
#include <iostream>
#include <vector>
using namespace std;

// Performs the Fast Fourier Transforms
vector<complex<double>> fft(const vector<complex<double>> &arr,
                            bool inverse = false) {
    unsigned long n = arr.size();
    if (n == 1)
        return vector<complex<double>>(1, arr[0]);
    vector<complex<double>> unity_roots(n);
    for (int i = 0; i < n; i++) {
        double alpha = 2 * M_PI * i / n;
        unity_roots[i] =
            complex<double>(cos(alpha), !inverse ? sin(alpha) : -sin(alpha));
    }
    vector<complex<double>> arr_even(n / 2), arr_odd(n / 2);
    for (int i = 0; i < n / 2; i++) {
        arr_even[i] = arr[i * 2];
        arr_odd[i] = arr[i * 2 + 1];
    }
    vector<complex<double>> fft_even = fft(arr_even, inverse);
    vector<complex<double>> fft_odd = fft(arr_odd, inverse);
    vector<complex<double>> fft(n);
    if (inverse)
        for (auto &el : fft_even)
            el *= fft_even.size();
    if (inverse)
        for (auto &el : fft_odd)
            el *= fft_odd.size();
    for (int k = 0; k < n / 2; k++) {
        fft[k] = fft_even[k] + unity_roots[k] * fft_odd[k];
        fft[k + n / 2] = fft_even[k] - unity_roots[k] * fft_odd[k];
    }
    if (inverse)
        for (auto &el : fft)
            el /= fft.size();
    return fft;
}

// Performs the Number Theoretic Transforms
vector<long long> ntt(vector<long long> a, bool invert = false) {
    const long long mod = 7340033;
    const long long root_forward = 5;
    const long long root_inverse = 4404020;
    const long long root_power = 1 << 20;

    unsigned long n = a.size();

    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j >= bit; bit >>= 1)
            j -= bit;
        j += bit;
        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        long long w_len = invert ? root_inverse : root_forward;
        for (int i = len; i < root_power; i <<= 1)
            w_len = w_len * 1ll * w_len % mod;
        for (int i = 0; i < n; i += len) {
            int w = 1;
            for (int j = 0; j < len / 2; ++j) {
                int u = a[i + j], v = int(a[i + j + len / 2] * 1ll * w % mod);
                a[i + j] = u + v < mod ? u + v : u + v - mod;
                a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + mod;
                w = int(w * 1ll * w_len % mod);
            }
        }
    }
    auto mod_inverse = [](long long x, long long modulus) {
        long long cumulative = x, result = 1;
        for (long long power = modulus - 2; power > 0; power /= 2) {
            if (power % 2 == 1)
                result = (result * cumulative) % modulus;
            cumulative = (cumulative * cumulative) % modulus;
        }
        return result;
    };

    if (invert) {
        int nrev = mod_inverse(n, mod);
        for (int i = 0; i < n; ++i)
            a[i] = int(a[i] * 1ll * nrev % mod);
    }

    return a;
}

// The full Polynomial Class with fast NTT

typedef long long ll;
typedef vector<long long> vll;

class Polynomial {
    static const int root = 973800541;
    static const int root_1 = 595374802;
    static const int root_pw = 1 << 20;
    static const ll MOD = 998244353;

    static ll __mod_pow(ll a, ll n) {
        int res = 1;
        for (a %= MOD; n > 0; n >>= 1) {
            if (n & 1)
                res = (res * 1ll * a) % MOD;
            a = (a * 1ll * a) % MOD;
        }
        return res;
    }

   public:
    int order;
    vll coeff;

    explicit Polynomial(vll coefficients) {
        order = coefficients.size() - 1;
        coeff = coefficients;
        this->resize(order);
    }
    Polynomial(const Polynomial &copy) {
        order = copy.order;
        coeff = vll(copy.coeff);
    }
    void resize(int order) {
        int size;
        for (size = 1; size < order + 1; size *= 2)
            ;
        coeff.resize(size);
    }

    void ntt(bool invert = false) {
        int n = coeff.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1)
                j ^= bit;
            j ^= bit;
            if (i < j)
                swap(coeff[i], coeff[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            int wlen = invert ? root_1 : root;
            for (int i = len; i < root_pw; i <<= 1)
                wlen = (int)(1LL * wlen * wlen % MOD);
            for (int i = 0; i < n; i += len) {
                int w = 1;
                for (int j = 0; j < len / 2; j++) {
                    int u = coeff[i + j],
                        v = (ll)((coeff[i + j + len / 2] * 1ll * w) % MOD);
                    coeff[i + j] = u + v < MOD ? u + v : u + v - MOD;
                    coeff[i + j + len / 2] = u - v >= 0 ? u - v : u - v + MOD;
                    w = (int)((w * 1ll * wlen) % MOD);
                }
            }
        }
        if (invert) {
            int n_1 = __mod_pow(n, MOD - 2);
            for (ll &x : coeff)
                x = (ll)((x * 1ll * n_1) % MOD);
        }
    }

    friend Polynomial operator*(const Polynomial &a, const Polynomial &b) {
        Polynomial x(a), y(b);
        int order = a.order + b.order;
        x.resize(order);
        y.resize(order);
        x.ntt();
        y.ntt();
        int size = x.coeff.size();
        vll poly(size);
        for (int i = 0; i < size; i++) {
            poly[i] = (x.coeff[i] * y.coeff[i]) % MOD;
        }
        Polynomial res(poly);
        res.ntt(true);
        res.order = order;
        return res;
    }

    friend Polynomial operator^(const Polynomial &a, ll pow) {
        Polynomial x(a);
        int order = a.order * pow;
        x.resize(order);
        x.ntt();
        int size = x.coeff.size();
        vll poly(size);
        for (int i = 0; i < size; i++)
            poly[i] = __mod_pow(x.coeff[i], pow);
        Polynomial res(poly);
        res.ntt(true);
        res.order = order;
        return res;
    }
};

#endif  // CODE_FASTFOURIER_H