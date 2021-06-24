#include "template.hpp"

void ntt(vector<int> &coeff, bool invert = false, const int MOD = 998244353,
         const int root = 565042129, const int root_inv = 950391366,
         const int root_pow = 1 << 20) {
    int n = (int)coeff.size();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(coeff[i], coeff[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        int wlen = invert ? root_inv : root;
        for (int i = len; i < root_pow; i <<= 1)
            wlen = (int)(1LL * wlen * wlen % MOD);

        for (int i = 0; i < n; i += len) {
            int w = 1;
            for (int j = 0; j < len / 2; j++) {
                int u = coeff[i + j],
                    v = (int)(1LL * coeff[i + j + len / 2] * w % MOD);
                coeff[i + j] = u + v < MOD ? u + v : u + v - MOD;
                coeff[i + j + len / 2] = u - v >= 0 ? u - v : u - v + MOD;
                w = (int)(1LL * w * wlen % MOD);
            }
        }
    }
    if (invert) {
        int n_1 = mod_inverse(n, MOD);
        for (auto &x : coeff)
            x = (int)(1LL * x * n_1 % MOD);
    }
}

void fft(vector<complex<double>> &coeff, bool invert = false) {
    int n = (int)coeff.size();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(coeff[i], coeff[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * M_PI / len * (invert ? -1 : 1);
        complex<double> wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            complex<double> w(1);
            for (int j = 0; j < len / 2; j++) {
                complex<double> u = coeff[i + j],
                                v = coeff[i + j + len / 2] * w;
                coeff[i + j] = u + v;
                coeff[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (complex<double> &x : coeff)
            x /= n;
    }
}

void zeta_transform(vector<ll> &a, const int MOD = 998244353) {
    const size_t n = a.size();
    for (size_t w = 1; w != n; w *= 2) {
        for (size_t k = 0; k != n; k += w * 2) {
            for (size_t i = 0; i != w; i += 1) {
                a[k + i] = (a[k + i] + a[k + w + i]) % MOD;
            }
        }
    }
}

void mobius_transform(vector<ll> &a, const int MOD = 998244353) {
    const size_t n = a.size();
    for (size_t w = 1; w != n; w *= 2) {
        for (size_t k = 0; k != n; k += w * 2) {
            for (size_t i = 0; i != w; i += 1) {
                a[k + i] = (a[k + i] - a[k + w + i] + MOD) % MOD;
            }
        }
    }
}

void walsh_hadamard_transform(vector<ll> &a, const int MOD = 998244353) {
    const size_t n = a.size();
    for (size_t w = 1; w != n; w *= 2) {
        for (size_t k = 0; k != n; k += w * 2) {
            for (size_t i = 0; i != w; i += 1) {
                const ll x = a[k + i];
                const ll y = a[k + w + i];
                a[k + i] = (x + y) % MOD;
                a[k + w + i] = (x - y + MOD) % MOD;
            }
        }
    }
}
