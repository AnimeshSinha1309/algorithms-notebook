#include "template.hpp"

struct DirectedGraph {
    int size, curr;
    vector<unordered_set<int>> adjacent_f, adjacent_r;
    vector<vector<int>> comp_nodes;
    vector<int> order, comp;
    vector<bool> visited;

    DirectedGraph(int n) {
        size = n, curr = 0;
        order.resize(size);
        comp.resize(size);
        comp_nodes.resize(size + 1);
        adjacent_f.resize(size);
        adjacent_r.resize(size);
        visited.resize(size);
    }
    void add_edge(int v1, int v2) {
        adjacent_f[v1].insert(v2);
        adjacent_r[v2].insert(v1);
    }
    void _scc_dfs1(int u) {
        visited[u] = true;
        for (auto w : adjacent_f[u])
            if (!visited[w])
                _scc_dfs1(w);
        order.push_back(u);
    }
    void _scc_dfs2(int u) {
        visited[u] = 1;
        comp[u] = curr;
        comp_nodes[curr].push_back(u);
        for (auto w : adjacent_r[u])
            if (!visited[w])
                _scc_dfs2(w);
    }
    void stongly_connected_components() {
        fill(visited.begin(), visited.end(), false);
        order.clear();
        for (int i = 0; i < size; i++)
            if (!visited[i])
                _scc_dfs1(i);
        fill(visited.begin(), visited.end(), false);
        reverse(order.begin(), order.end());
        curr = 0;
        // for (auto u : order)
        //     if (!visited[u])
        //         comp_nodes[++curr].clear(), _scc_dfs2(u);
    }
};

struct Satisfiability : DirectedGraph {
    vector<bool> val;
    Satisfiability(int size) : DirectedGraph(2 * size) {
        val = vector<bool>(size, false);
    }

    bool solvable() {
        stongly_connected_components();
        for (size_t i = 0; i < val.size(); i++)
            if (comp[var(i)] == comp[NOT(var(i))])
                return false;
        return true;
    }
    vector<bool> solve() {
        fill(val.begin(), val.end(), false);
        for (size_t i = 0; i < val.size(); i++) {
            val[i] = comp[var(i)] > comp[NOT(var(i))];
        }
        return val;
    }
    int var(int x) {
        return x << 1;
    }
    int NOT(int x) {
        return x ^ 1;
    }
    void add_imp(int v1, int v2) {
        add_edge(v1, v2);
        add_edge(1 ^ v2, 1 ^ v1);
    }
    void add_equiv(int v1, int v2) {
        add_imp(v1, v2);
        add_imp(v2, v1);
    }
    void add_or(int v1, int v2) {
        add_edge(1 ^ v1, v2);
        add_edge(1 ^ v2, v1);
    }
    void add_xor(int v1, int v2) {
        add_or(v1, v2);
        add_or(1 ^ v1, 1 ^ v2);
    }
    void add_nand(int v1, int v2) {
        add_edge(v1, 1 ^ v2);
        add_edge(v2, 1 ^ v1);
    }
    void add_true(int v1) {
        add_edge(1 ^ v1, v1);
    }
    void add_false(int v1) {
        add_edge(v1, 1 ^ v1);
    }
    void add_and(int v1, int v2) {
        add_true(v1);
        add_true(v2);
    }
};