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

struct SuffixArray {
// { { left subarray class, right subarray class }, start idx in original string }
typedef pair<PII, int> data;
#define alphabet 256

    string s;
    int n;
    VI classes;
    vector<data> idx;
    VI lcp;
    VVI sparselcp;

    SuffixArray(const string &str) {
        s = str + " ";
        n = s.size();
        build_SA();
    }

    void build_SA() {
        classes = VI(n);

        {
            // building the initial classes using counting sort
            vector<bool> occur(alphabet, 0);
            for (int i = 0; i < n; i++) occur[s[i]] = true;
            VI init_class(alphabet, -1);
            int rank = 0;
            for (int i = 0; i < alphabet; i++)
                if (occur[i])
                    init_class[i] = rank++;
            for (int i = 0; i < n; i++) classes[i] = init_class[s[i]];
        }

        // the size of the subarray that's going to be sorted in the next iteration
        int sz = 1;

        vector<data> idx_temp(n);
        idx = vector<data>(n);

        while (true) {
            {
                // build the sorted array of indexes
                VI countsecond(n + 1, 0);
                VI countfirst(n + 1, 0);

                for (int i = 0; i < n; i++) {
                    int cnxt = classes[(sz / 2 + i) % n];
                    int ci = classes[i];
                    countsecond[cnxt + 1]++;
                    countfirst[ci + 1]++;
                }

                // simultaneous radix sort+counting sort
                for (int i = 1; i < n; i++)
                    countsecond[i] += countsecond[i - 1], countfirst[i] += countfirst[i - 1];

                // insert in order sorted by second class
                for (int i = 0; i < n; i++) {
                    int cnxt = classes[(sz / 2 + i) % n];
                    int ci = classes[i];
                    idx_temp[countsecond[cnxt]++] = { { ci, cnxt }, i };
                }

                // now sort by the first class
                for (int i = 0; i < n; i++) {
                    int ci = idx_temp[i].first.first;
                    idx[countfirst[ci]++] = idx_temp[i];
                }
            }

            // assign equivalence classes based on sorted idx
            classes = VI(n);
            int rank = 0;
            for (int i = 0; i < n; i++) {
                int index = idx[i].second;

                if (i > 0 and idx[i].first > idx[i - 1].first) classes[index] = ++rank;
                else classes[index] = rank;
            }

            if (rank == n - 1) break;

            sz <<= 1;
        }
    }

    void build_lcp() {
        lcp = VI(n);

        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pi = classes[i];
            int j = idx[pi - 1].second;
            while (s[i + k] == s[j + k]) k++;

            lcp[pi] = k;
            k = max(k - 1, 0);
        }

        int logn = ceil(log2(n)) + 1;
        sparselcp = VVI(logn, VI(n, -1));
        for (int i = 0; i < n; i++) sparselcp[0][i] = lcp[i];

        int sz = 1;
        for (int lev = 1; lev < logn; lev++) {
            for (int i = 0; i < n; i++) {
                int j = i + sz;
                if (j >= n) break;
                int val = sparselcp[lev - 1][j];
                if (val == -1) break;
                sparselcp[lev][i] = min(sparselcp[lev - 1][i], val);
            }
            sz <<= 1;
        }
    }

    // gets the lcp of the suffixes at position i
    // and position j
    int getlcp(int i, int j) {
        if (i == j) return n - i;

        int pi = classes[i], pj = classes[j];
        if (pi > pj) swap(pi, pj);

        pi++;
        int gap = log2(pj - pi + 1);

        return min(sparselcp[gap][pi], sparselcp[gap][pj - (1 << gap) + 1]);
    }

    // i..j, p..q must be the indices in the string s
    int comp(int i, int j, int p, int q) {
        int _lcp = getlcp(i, p);
        int len1 = j - i + 1, len2 = q - p + 1;
        int minlen = min(len1, len2);

        if (minlen <= _lcp) return len1 - len2;

        char c1 = s[i + _lcp], c2 = s[p + _lcp];
        return c1 - c2;
    }
};

#endif  // CODE_SMARTSTRING_H
