class Array_Hash {
    ll p, m, n;
    v pre, suf, powers, inverse_powers;

   public:
    Array_Hash(v &arr) {
        this->p = 1e6+3;  // array elements must be less than p 
        this->m = 1e9 + 9;  // large prime
        ll n = sz(arr);
        this->n = n;
        this->pre.resize(n);
        this->suf.resize(n);
        this->powers.resize(n);
        this->inverse_powers.resize(n);
        this->powers[0] = 1;
        ll p1 = p;
        for (ll i = 1; i < n; i++) {
            this->powers[i] = (this->powers[i - 1] * p) % this->m;
        }
        for (ll i = 0; i < n; i++) {
            this->inverse_powers[i] =
                power(this->powers[i], this->m - 2, this->m);
        }
        pre[0] = arr[0];
        for (ll i = 1; i < n; i++) {
            this->pre[i] = (this->pre[i - 1] + arr[i] * p1) % m;
            p1 *= p;
            if (p1 > m)
                p1 %= m;
        }
        this->suf[n - 1] = arr[n - 1];
        p1 = p;
        for (ll i = n - 2; i >= 0; i--) {
            this->suf[i] = (this->suf[i + 1] + arr[i] * p1) % m;
            p1 *= p;
            if (p1 > m)
                p1 %= m;
        }
    }
    ll prefix_hash(ll l, ll r) {
        ll ans;
        if (l != 0)
            ans = (pre[r] - pre[l - 1] + this->m) % this->m;
        else
            ans = pre[r];
        ans *= this->inverse_powers[l];
        if (ans > this->m)
            ans %= this->m;
        return ans;
    }
    ll suffix_hash(ll l, ll r) {
        ll ans, n = this->n;
        if (r == (n - 1))
            ans = this->suf[l];
        else
            ans = (this->suf[l] - this->suf[r + 1] + this->m) % this->m;
        ans *= this->inverse_powers[n - r - 1];
        if (ans > this->m)
            ans %= this->m;
        return ans;
    }
    bool is_palindrome(ll l, ll r) {
        return (prefix_hash(l, r) == suffix_hash(l, r));
    }
};