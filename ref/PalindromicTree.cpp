#include "template.hpp"

class PalindromicTree {
    const static long long MAXN = 100000;

   public:
    struct Node {
        int start, end;
        int length;
        int insert_edge[26];
        int suffix_edge;
    };

    Node root1, root2;
    Node tree[MAXN];
    int curr_node, ptr, size;
    string s;

    void insert(int idx) {
        int tmp = curr_node;
        while (true) {
            int curLength = tree[tmp].length;
            if (idx - curLength >= 1 and s[idx] == s[idx - curLength - 1])
                break;
            tmp = tree[tmp].suffix_edge;
        }
        if (tree[tmp].insert_edge[s[idx] - 'a'] != 0) {
            curr_node = tree[tmp].insert_edge[s[idx] - 'a'];
            return;
        }
        ptr++;
        tree[tmp].insert_edge[s[idx] - 'a'] = ptr;
        tree[ptr].length = tree[tmp].length + 2;
        tree[ptr].end = idx;
        tree[ptr].start = idx - tree[ptr].length + 1;
        tmp = tree[tmp].suffix_edge;
        curr_node = ptr;
        if (tree[curr_node].length == 1) {
            tree[curr_node].suffix_edge = 2;
            return;
        }
        while (true) {
            int cur_length = tree[tmp].length;
            if (idx - cur_length >= 1 and s[idx] == s[idx - cur_length - 1])
                break;
            tmp = tree[tmp].suffix_edge;
        }
        tree[curr_node].suffix_edge = tree[tmp].insert_edge[s[idx] - 'a'];
    }

    PalindromicTree(string st) {
        root1.length = -1, root1.suffix_edge = 1, root2.length = 0,
        root2.suffix_edge = 1, tree[1] = root1, tree[2] = root2, ptr = 2;
        curr_node = 1, s = st, size = st.size();
        for (int i = 0; i < size; i++)
            insert(i);
    }

    vpl get_palindromes() {
        vpl res(ptr - 2);
        for (int i = 3; i <= ptr; i++)
            res[i - 2] = {tree[i].start, tree[i].end};
        return res;
    }
};
