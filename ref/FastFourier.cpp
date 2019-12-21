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