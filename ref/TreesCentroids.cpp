#include "template.hpp"

class Tree {
   public:
    struct Node {
        vector<Node *> adjacent; Node *parent = nullptr;
        int start_time = 0, end_time = 0, subtree_size = 1;
        int depth = 0, height = 0, index = INT32_MAX;
    };
    vector<Node> list; Node *root = nullptr;
    vector<vector<Node *>> __anc;
    Tree(int n = 1e5) {
        list.resize(n); this->root = &list[0];
        for (int i = 0; i < n; i++) list[i].index = i;
    }
    void add_edge(int x, int y) {
        list[x].adjacent.push_back(&list[y]);
        list[y].adjacent.push_back(&list[x]);
    }
    Node *lca(Node *a, Node *b) {
        if (b->depth > a->depth) swap(a, b);
        for (int ptr = __anc[0].size() - 1; a->depth > b->depth && ptr >= 0; ptr--)
            if (__anc[a->index][ptr] != nullptr &&
                __anc[a->index][ptr]->depth >= b->depth)
                a = __anc[a->index][ptr];
        if (a == b) return a;
        for (long step = __anc[0].size() - 1; step >= 0; step--)
            if (__anc[a->index][step] != __anc[b->index][step])
                a = __anc[a->index][step], b = __anc[b->index][step];
        return a->parent;
    }
    Node *ancestor(Node *a, int degree) {
        ll target_depth = a->depth - degree;
        for (int ptr = __anc[0].size() - 1; a->depth > target_depth && ptr >= 0; ptr--)
            if (__anc[a->index][ptr] != nullptr &&
                __anc[a->index][ptr]->depth >= target_depth)
                a = __anc[a->index][ptr];
        return a;
    }
    int __build(Node *root = nullptr, int time = 0) {
        if (root == nullptr) root = this->root;
        root->start_time = time;
        for (auto child : root->adjacent) {
            if (child == root->parent) continue;
            child->parent = root;
            child->depth = root->depth + 1;
            time = __build(child, time + 1);
            root->height = max(root->height, child->height + 1);
            root->subtree_size += child->subtree_size;
        }
        root->end_time = time;
        return time;
    }
    void __build_lca_matrix() {
        int n = list.size();
        __anc = *new vector<vector<Node *>>(n, vector<Node *>(log2(n) + 1, nullptr));
        for (int i = 0; i < list.size(); i++) __anc[i][0] = list[i].parent;
        for (int level = 1; level < __anc[0].size(); level++)
            for (int i = 0; i < list.size(); i++) {
                if (__anc[i][level - 1] == nullptr) continue;
                __anc[i][level] = __anc[__anc[i][level - 1]->index][level - 1];
            }
    }
};

struct CentroidTree : Tree {
    vector<bool> __visited; vector<int> __dir_parents, __subtree_size; Tree base;
    void __dfs_centroid(int node) {
        __subtree_size[node] = 1;
        for (Node *next : base.list[node].adjacent)
            if (!__visited[next->index] && next->index != __dir_parents[node]) {
                __dir_parents[next->index] = node;
                __dfs_centroid(next->index);
                __subtree_size[node] += __subtree_size[next->index];
            }
    }
    int __get_centroid(int x) {
        __dir_parents[x] = 0; __dfs_centroid(x); int sz = __subtree_size[x];
        while (true) {
            pair<int, int> mx = {0, 0};
            for (Node *next : base.list[x].adjacent)
                if (!__visited[next->index] && next->index != __dir_parents[x])
                    mx = max(mx, {__subtree_size[next->index], next->index});
            if (mx.first * 2 > sz) x = mx.second; else return x;
        }
    }
    void __build_centroid(int node, Node *parent) {
        node = __get_centroid(node); list[node].parent = parent; __visited[node] = true;
        for (Node *next : base.list[node].adjacent)
            if (!__visited[next->index]) __build_centroid(next->index, &list[node]);
    }
    CentroidTree(Tree &tree) : Tree((int)tree.list.size()) {
        __visited = vector<bool>(tree.list.size());
        __subtree_size = vector<int>(tree.list.size());
        __dir_parents = vector<int>(tree.list.size());
        base = tree;
        __build_centroid(0, nullptr);
        for (auto el : list) {
            if (el.parent == nullptr) root = &list[el.index];
            else add_edge(el.index, el.parent->index);
        } __build(root);
    }
};

ll diameter(Tree tree) {
    ll n = tree.list.size() + 1; vbl visited(n + 1, false); vll distances(n + 1, -1);
    queue<pll> q; q.push({tree.root->index, 0});
    ll node_max = tree.root->index, distance_max = 0;
    while (!q.empty()) {
        auto node = q.front(); q.pop();
        if (node.second < distance_max) distance_max = node.second, node_max = node.first;
        for (auto neighbor : tree.list[node.first].adjacent)
            if (!visited[neighbor->index]) {
                auto d = node.second + 1;
                q.push({neighbor->index, d});
                visited[neighbor->index] = 1;
            }
    }
    visited = vbl(n + 1, false);
    q.push({node_max, 0}); distance_max = 0;
    while (!q.empty()) {
        auto node = q.front(); q.pop();
        maximize(distance_max, node.second);
        for (auto neighbor : tree.list[node.first].adjacent)
            if (!visited[neighbor->index]) {
                auto d = node.second + 1;
                q.push({neighbor->index, d});
                visited[neighbor->index] = 1;
            }
    }
}

struct HeavyLightDecomp : Tree {
    int chain_count = 1, narr;
    vector<int> subtree_size, chain, chain_head, chain_next;
    function<ll(int, int, ll)> answer;
    vector<int> pos;

    HeavyLightDecomp(int n, function<ll(int, int, ll)> &ans) : Tree(n) {
        subtree_size.resize(n); pos.resize(n); chain.resize(n);
        chain_head.resize(n); chain_next.resize(n); answer = ans;
    }

    void decompose(int node = 0, int parent = -1) {
        pos[node] = ++narr, chain[node] = chain_count; int big = 0;
        for (Node *adj : list[node].adjacent) {
            int u = adj->index;
            if (u == parent) continue;
            else if (!big) big = u;
            else if (subtree_size[u] > subtree_size[big]) big = u;
        }
        if (big) decompose(big, node);
        for (Node *adj : list[node].adjacent) {
            int u = adj->index;
            if (u == parent || u == big) continue;
            ++chain_count, chain_head[chain_count] = u,
                           chain_next[chain_count] = node;
            decompose(u, node);
        }
    }
    // Build Segment Tree using indices of pos array
    // Update ans using Range queries on said segment tree
    int query_up(int r, int q) {
        int ans = 0, t;
        while (chain[q] != chain[r]) {
            t = chain[q];
            ans = answer(pos[chain_head[t]], pos[q], ans);
            q = chain_next[t];
        }
        ans = answer(pos[r], pos[q], ans);
        return ans;
    }
};