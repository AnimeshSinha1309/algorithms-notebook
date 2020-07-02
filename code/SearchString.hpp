#ifndef CODE_SMARTSTRING_H
#define CODE_SMARTSTRING_H

#include "../ref/template.hpp"

class KMPstring {
    string pattern;
    vll lps;

   public:
    explicit KMPstring(const string &pattern) {
        this->pattern = pattern;
        ll m = pattern.size();
        lps = vll(m + 1, 0);
        ll i = 0, j = -1;
        lps[0] = -1;
        while (i < m) {
            while (j >= 0 && pattern[i] != pattern[j])
                j = lps[j];
            i++, j++;
            lps[i] = j;
        }
    }
    vll match(const string &text) {
        ll n = text.size(), m = pattern.size();
        vll matches, m_length(n);
        ll i = 0, j = 0;
        while (i < n) {
            while (j >= 0 && text[i] != pattern[j])
                j = lps[j];
            i++, j++;
            m_length[i - 1] = j;
            if (j == m) {
                matches.push_back(i - m);
                j = lps[j];
            }
        }
        return move(matches);  // or m_length
    }
};

class SuffixArray {
   public:
    string s;
    int n, __log_n;
    vector<int> sa;            // Suffix Array
    vector<vector<int>> ra;    // Rank Array
    vector<vector<int>> _lcp;  // Longest Common Prefix
    vector<int> __msb, __dollar;

    SuffixArray(string st) {
        n = st.size();
        __log_n = log2(n) + 2;
        ra = vector<vector<int>>(__log_n, vector<int>(n));
        sa = vector<int>(n);

        __msb = vector<int>(n);
        int mx = -1;
        for (int i = 0; i < n; i++) {
            if (i >= (1 << (mx + 1)))
                mx++;
            __msb[i] = mx;
        }
        this->s = st;
        build_SA();
    }

    void __counting_sort(int l, int k) {
        int maxi = max(300, n);
        vector<int> count(maxi, 0), temp_sa(n, 0);
        for (int i = 0; i < n; i++) {
            int idx = (i + k < n ? ra[l][i + k] : 0);
            count[idx]++;
        }
        for (int i = 0, sum = 0; i < maxi; i++) {
            int t = count[i];
            count[i] = sum;
            sum += t;
        }
        for (int i = 0; i < n; i++) {
            int idx = sa[i] + k < n ? ra[l][sa[i] + k] : 0;
            temp_sa[count[idx]++] = sa[i];
        }
        sa = temp_sa;
    }

    void build_SA() {
        for (int i = 0; i < n; i++)
            ra[0][i] = s[i];
        for (int i = 0; i < n; i++)
            sa[i] = i;
        for (int i = 0; i < __log_n - 1; i++) {
            int k = (1 << i);
            if (k >= n)
                break;
            __counting_sort(i, k);
            __counting_sort(i, 0);
            int rank = 0;
            ra[i + 1][sa[0]] = rank;
            for (int j = 1; j < n; j++)
                if (ra[i][sa[j]] == ra[i][sa[j - 1]] &&
                    ra[i][sa[j] + k] == ra[i][sa[j - 1] + k])
                    ra[i + 1][sa[j]] = rank;
                else
                    ra[i + 1][sa[j]] = ++rank;
        }
    }
    void build_LCP() {
        _lcp = vector<vector<int>>(__log_n, vector<int>(n));
        for (int i = 0; i < n - 1; i++) {  // Build the LCP array in O(NlogN)
            int x = sa[i], y = sa[i + 1], k, ret = 0;
            for (k = __log_n - 1; k >= 0 && x < n && y < n; k--) {
                if ((1 << k) >= n)
                    continue;
                if (ra[k][x] == ra[k][y])
                    x += 1 << k, y += 1 << k, ret += 1 << k;
            }
            if (ret >= __dollar[sa[i]] - sa[i])
                ret = __dollar[sa[i]] - sa[i];
            _lcp[0][i] = ret;  // LCP[i] shouldn’t exceed __dollar[sa[i]]
        }  // __dollar[i] : index of __dollar to the right of i.
        _lcp[0][n - 1] = 10 * n;
        for (int i = 1; i < __log_n; i++) {  // O(1) RMQ structure in O(NlogN)
            int add = (1 << (i - 1));
            if (add >= n)
                break;  // small optimization
            for (int j = 0; j < n; j++)
                if (j + add < n)
                    _lcp[i][j] = min(_lcp[i - 1][j], _lcp[i - 1][j + add]);
                else
                    _lcp[i][j] = _lcp[i - 1][j];
        }
    }

    int lcp(int x, int y) {
        // O(1) LCP. x & y are indexes of the suffix in sa!
        if (x == y)
            return __dollar[sa[x]] - sa[x];
        if (x > y)
            swap(x, y);
        y--;
        int idx = __msb[y - x + 1], sub = (1 << idx);
        return min(_lcp[idx][x], _lcp[idx][y - sub + 1]);
    }

    bool equal(int i, int j, int p, int q) {
        if (j - i != q - p)
            return false;
        int idx = __msb[j - i + 1], sub = (1 << idx);
        return ra[idx][i] == ra[idx][p] &&
               ra[idx][j - sub + 1] == ra[idx][q - sub + 1];
    }  // Note : Do not forget to add a terminating ’$’
};

#endif  // CODE_SMARTSTRING_H
