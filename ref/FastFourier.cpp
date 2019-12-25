#include "template.hpp"
const double PI = acos(-1);

#ifdef IS_FFT
    using cd = complex<double>;
#else
    using cd = int;
#endif
// use llround(a[i].real()) when printing FFT output
struct Polynomial {
    static const int root = 565042129;
    static const int root_1 = 950391366;
    static const int root_pw = 1 << 20;
    static const int mod = 998244353;

    static int __mod_pow(int a, int n) {
        int res = 1;
        for (a %= mod; n > 0; n >>= 1) {
            if (n & 1) res = (int)((1LL * res * a) % mod);
            a = (int)((a * 1ll * a) % mod);
        } return res;
    }
    int order;
    vector<cd> coeff;
    explicit Polynomial() : order(0), coeff(vector<cd>(0)) {
    }
    explicit Polynomial(vector<cd> coefficients)
        : order((int)coefficients.size()), coeff(coefficients) {
    }
    Polynomial(const Polynomial &copy)
        : order(copy.order), coeff(vector<cd>(copy.coeff)) {
    }
    void resize(int nOrder) {
        int size = 1 << (ll)ceil(log2(nOrder));
        coeff.resize(size, 0);
    }

#ifdef IS_FFT
    void fft(bool invert = false) {
        int n = (int)coeff.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(coeff[i], coeff[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2 * PI / len * (invert ? -1 : 1);
            cd wlen(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len) {
                cd w(1);
                for (int j = 0; j < len / 2; j++) {
                    cd u = coeff[i + j], v = coeff[i + j + len / 2] * w;
                    coeff[i + j] = u + v;
                    coeff[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }
        if (invert) { for (cd &x : coeff) x /= n; }
    }
#else
    void fft(bool invert = false) {
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
            int wlen = invert ? root_1 : root;
            for (int i = len; i < root_pw; i <<= 1)
                wlen = (int)(1LL * wlen * wlen % mod);
            for (int i = 0; i < n; i += len) {
                int w = 1;
                for (int j = 0; j < len / 2; j++) {
                    int u = coeff[i + j],
                        v = (int)(1LL * coeff[i + j + len / 2] * w % mod);
                    coeff[i + j] = u + v < mod ? u + v : u + v - mod;
                    coeff[i + j + len / 2] = u - v >= 0 ? u - v : u - v + mod;
                    w = (int)(1LL * w * wlen % mod);
                }
            }
        }
        if (invert) {
            int n_1 = __mod_pow(n, mod - 2);
            for (auto &x : coeff)
                x = (int)(1LL * x * n_1 % mod);
        }
    }
#endif
    friend Polynomial operator*(const Polynomial &a, const Polynomial &b) {
        Polynomial x(a), y(b);
        int order = a.order + b.order;
        order = 1 << (ll)ceil(log2(order));
        x.resize(order), y.resize(order);
        x.fft(), y.fft();

        for (int i = 0; i < order; i++) {
#ifdef IS_FFT
            x.coeff[i] = (x.coeff[i] * y.coeff[i]);
#else
            x.coeff[i] = (int)((1ll * x.coeff[i] * y.coeff[i]) % mod);
#endif
        }
        x.fft(true);
        return x;
    }

    friend Polynomial operator^(const Polynomial &a, int power) {
        Polynomial x(a);
        int order = a.order * power;
        x.resize(order);
        x.fft();
        int size = (int)x.coeff.size();
        vector<cd> poly(size);
        Polynomial res(poly);
#ifdef IS_FFT
        for (int i = 0; i < size; i++)
            poly[i] = pow(x.coeff[i], power);
#else
        for (int i = 0; i < size; i++)
            poly[i] = __mod_pow(x.coeff[i], power);
#endif
        res.fft(true);
        res.order = order;
        return res;
    }
};

// Code for finding closest match by Hamming distance of r in s |r| <= |s|
// we reverse polynomial r and multiply with s
// for (ll i = (int)r.size() - 1 - 1; i < s.size(); i++)
//     res[i] += z.coeff[i]; // z is the multiplication result
// answers contained in res[sz(r) - 1] to res[sz(s) - 1]