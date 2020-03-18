#ifndef CODE_DISJOINTSETUNIONFIND_H
#define CODE_DISJOINTSETUNIONFIND_H

#include "../ref/template.hpp"

struct DisjointSetTree {
    ll comp_count;
    vector<ll> parent, comp_size;
    set<ll> roots;

    DisjointSetTree(int n) {
        comp_count = n;
        parent.resize(n);
        comp_size.resize(n, 1);
        iota(parent.begin(), parent.end(), 0);
        for (int i = 0; i < n; i++) {
            roots.insert(i);
        }
    }

    int find(int u) {
        if (parent[u] == u)
            return parent[u];
        return parent[u] = find(parent[u]);
    }

    bool merge(int u, int v) {
        u = find(u), v = find(v);
        if (u == v)
            return false;
        parent[u] = v;
        comp_size[v] += comp_size[u];
        comp_size[u] = 0;
        roots.erase(u);
        comp_count--;
        return true;
    }
};

class DynamicConnectivity {
    void __dfs(int v, int l, int r, vector<long long>& res) {
        long long last_ans = answer;
        int state = save_ptr;
        for (auto query : tree[v])
            merge(query);
        if (l == r - 1)
            res[l] = answer;
        else {
            int m = (l + r) / 2;
            __dfs(v * 2 + 1, l, m, res);
            __dfs(v * 2 + 2, m, r, res);
        }
        while (save_ptr != state)
            rollback();
        answer = last_ans;
    };

   public:
    int size_nodes;
    int size_query;

    struct Node {
        long long parent, comp_size = 1;
    };
    long long answer = 0;
    vector<Node> data;
    vector<long long*> saved_object;
    vector<long long> saved_value;
    int save_ptr = 0;

    struct Query {
        int u, v;
        Query(pair<int, int> p = {0, 0}) {
            u = p.first, v = p.second;
        }
    };
    vector<vector<Query>> tree;

    DynamicConnectivity(int n = 600000, int q = 300000) {
        size_nodes = n;
        size_query = q;
        int tree_size = 1;
        while (tree_size < q)
            tree_size <<= 1;
        data = vector<Node>(n);
        tree = vector<vector<Query>>(2 * tree_size);
        saved_object = vector<long long*>(4 * q);
        saved_value = vector<long long>(4 * q);
        for (int i = 0; i < n; i++) {
            data[i].parent = i;
        }
        // Storing the initial answer
        answer = n;
    }

    void change(long long& x, long long y) {
        saved_object[save_ptr] = &x;
        saved_value[save_ptr] = x;
        x = y;
        save_ptr++;
    }

    void rollback() {
        save_ptr--;
        (*saved_object[save_ptr]) = saved_value[save_ptr];
    }

    int find(int x) {
        if (data[x].parent == x)
            return x;
        return find(data[x].parent);
    }

    void merge(const Query& q) {
        int x = find(q.u);
        int y = find(q.v);
        if (x == y)
            return;
        if (data[x].comp_size < data[y].comp_size)
            swap(x, y);
        change(data[y].parent, x);
        change(data[x].comp_size, data[x].comp_size + data[y].comp_size);
        // Changing the Answer on query
        change(answer, answer - 1);
    }

    void add(int l, int r, Query edge, int node = 0, int x = 0, int y = -1) {
        if (y == -1)
            y = size_query;
        if (l >= r)
            return;
        if (l == x && r == y)
            tree[node].push_back(edge);
        else {
            int m = (x + y) / 2;
            add(l, min(r, m), edge, node * 2 + 1, x, m);
            add(max(m, l), r, edge, node * 2 + 2, m, y);
        }
    }

    vector<long long> solve(int v = 0, int l = 0, int r = -1) {
        if (r == -1)
            r = size_query;
        vector<long long> vec(size_query);
        if (size_query > 0)
            __dfs(v, l, r, vec);
        return vec;
    }

    DynamicConnectivity(int n, vector<Query> queries)
        : DynamicConnectivity(n, queries.size()) {
        map<pair<int, int>, int> last;
        for (int i = 0; i < size_query; i++) {
            pair<int, int> p(queries[i].u, queries[i].v);
            if (last.count(p)) {
                add(last[p], i, queries[i]);
                last.erase(p);
            } else {
                last[p] = i;
            }
        }
        for (auto x : last)
            add(x.second, size_query, x.first);
    }
};

#endif  // CODE_DISJOINTSETUNIONFIND_H