#include "template.hpp"

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
    string s;
    int n, __log_n;
    vector<int> sa;            // Suffix Array
    vector<vector<int>> ra;    // Rank Array
    vector<vector<int>> _lcp;  // Longest Common Prefix
    vector<int> __msb, __dollar;

    SuffixArray(string st) {
        n = st.size();
        __log_n = log2(n) + 1;
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

// To check substring/LCS, run the string on the automaton.  Each path in the
// automaton is a substring(if it ends in a terminal node, it is a suffix). To
// find occurences of a string, run it on the automaton, and the number of its
// occurences would be number of ways to reach a terminal node. Or, we can keep
// reverse edges of suffix links(all prefixes for that substring), and number of
// ways to reach a root, would be the answer(can be used to print all answers)
struct AhoCorasick {
    vector<int> sufflink, out;
    vector<map<char, int>> trie;  // call findnextstate
    AhoCorasick() {
        out.resize(1);
        trie.resize(1);
    }
    inline void insert(string &s) {
        int curr = 0;  // clear to reinit
        for (int i = 0; i < s.size(); i++) {
            if (!trie[curr].count(s[i])) {
                trie[curr][s[i]] = trie.size();
                trie.push_back(map<char, int>());
                out.push_back(0);
            }
            curr = trie[curr][s[i]];
        }
        ++out[curr];
    }
    inline void build_automation() {
        sufflink.resize(trie.size());
        queue<int> q;
        for (auto x : trie[0]) {
            sufflink[x.second] = 0;
            q.push(x.second);
        }
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            for (auto x : trie[curr]) {
                q.push(x.second);
                int tmp = sufflink[curr];
                while (!trie[tmp].count(x.first) && tmp)
                    tmp = sufflink[tmp];
                if (trie[tmp].count(x.first))
                    sufflink[x.second] = trie[tmp][x.first];
                else
                    sufflink[x.second] = 0;
                out[x.second] += out[sufflink[x.second]];
            }
        }
    }
    int find_next_state(int curr, char ch) {
        while (curr && !trie[curr].count(ch))
            curr = sufflink[curr];
        return (!trie[curr].count(ch)) ? 0 : trie[curr][ch];
    }
    int query(string &s) {
        int ans = 0;
        int curr = 0;
        for (int i = 0; i < s.size(); i++) {
            curr = find_next_state(curr, s[i]);
            ans += out[curr];
        }
        return ans;
    }
    void clear() {
        trie.clear();
        sufflink.clear();
        out.clear();
        out.resize(1);
        trie.resize(1);
    }
};

// To check substring/LCS, run the string on the automaton. Each path in the
// automaton is a substring(if it ends in a terminal node, it is a suffix) To
// find occurences of a string, run it on the automaton, and the number of its
// occurences would be number of ways to reach a terminal node. Or, we can keep
// reverse edges of suffix links(all prefixes for that substring), and number of
// ways to reach a root, would be the answer(can be used to print all answers)
struct SuffixAutomaton {
    vector<map<char, int>> edges;
    vector<int> link, length;  // length[i]: longest string in i-th class
    int last;                  // index of equivalence class of whole string
    SuffixAutomaton(string s) {
        edges.push_back(map<char, int>());
        link.push_back(-1);
        length.push_back(0);
        last = 0;
        for (int i = 0; i < s.size(); i++) {
            edges.push_back(map<char, int>());
            length.push_back(i + 1);
            link.push_back(0);
            int r = edges.size() - 1;
            int p = last;
            while (p >= 0 && edges[p].find(s[i]) == edges[p].end())
                edges[p][s[i]] = r, p = link[p];
            if (p != -1) {
                int q = edges[p][s[i]];
                if (length[p] + 1 == length[q])
                    link[r] = q;
                else {
                    edges.push_back(edges[q]);
                    length.push_back(length[p] + 1);
                    link.push_back(link[q]);
                    int qq = edges.size() - 1;
                    link[q] = qq;
                    link[r] = qq;
                    while (p >= 0 && edges[p][s[i]] == q)
                        edges[p][s[i]] = qq, p = link[p];
                }
            }
            last = r;
        }
        vector<int> terminals;
        int p = last;
        while (p > 0)
            terminals.push_back(p), p = link[p];
    }
};