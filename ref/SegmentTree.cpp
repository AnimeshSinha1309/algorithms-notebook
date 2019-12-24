#include "template.hpp"

template <typename Type>
struct LazySegtree {
    int size;
    vector<Type> tree, lazy;
    Type _default;
    function<Type(Type, Type)> _operation;
    function<Type(Type, Type)> _setter;

    void split(int node) {
        lazy[2 * node] = _setter(lazy[2 * node], lazy[node]);
        tree[2 * node] = _setter(tree[2 * node], lazy[node]);
        lazy[2 * node + 1] = _setter(lazy[2 * node + 1], lazy[node]);
        tree[2 * node + 1] = _setter(tree[2 * node + 1], lazy[node]);
        lazy[node] = _default;
    }
    void merge(int node) { tree[node] = _operation(tree[2 * node], tree[2 * node + 1]); }

    LazySegtree(int n, const function<Type(Type, Type)> &op,
                const function<Type(Type, Type)> &set, const Type identity) {
        for (size = 1; size < n; size <<= 1);
        _setter = set, _operation = op, _default = identity;
        tree.assign(2 * size, _default);
        lazy.assign(2 * size, _default);
    }

    void modify(int l, int r, Type delta, int node = 1, int x = 0, int y = -1) {
        if (y == -1) y = size;
        if (r <= x || l >= y) return;
        if (l <= x && y <= r) {
            lazy[node] = _setter(lazy[node], delta);
            tree[node] = _setter(tree[node], delta);
            return;
        }
        split(node); modify(l, r, delta, 2 * node, x, (x + y) / 2);
        modify(l, r, delta, 2 * node + 1, (x + y) / 2, y); merge(node);
    }
    Type query(int l, int r, int node = 1, int x = 0, int y = -1) {
        if (y == -1) y = size;
        if (r <= x || l >= y) return _default;
        if (l <= x && y <= r) return tree[node];
        split(node); Type lres = query(l, r, 2 * node, x, (x + y) / 2);
        Type rres = query(l, r, 2 * node + 1, (x + y) / 2, y); merge(node);
        return _operation(lres, rres);
    }
};

template <typename Type>
struct ImplicitSegupdate {
    struct Node {
        Type data = 0;
        Node *l_ptr = nullptr, *r_ptr = nullptr;
        Node *l_child() { if (l_ptr == nullptr) l_ptr = new Node, r_ptr = new Node; return l_ptr; }
        Node *r_child() { if (r_ptr == nullptr) l_ptr = new Node, r_ptr = new Node; return r_ptr; }
    };
    int size; Node *root; function<Type(Type, Type)> _setter;
    ImplicitSegupdate(int n, const function<Type(Type, Type)> &set) {
        for (size = 1; size < n; size <<= 1) ;
        _setter = set; root = new Node;
    }
    void modify(int l, int r, Type delta, Node *node = nullptr, int x = 0,
                int y = -1) {
        if (node == nullptr) node = root, y = size;
        if (r <= x || l >= y) return;
        if (l <= x && y <= r) { node->data = _setter(node->data, delta); return; }
        modify(l, r, delta, node->l_child(), x, (x + y) / 2);
        modify(l, r, delta, node->r_child(), (x + y) / 2, y);
    }
    Type query(int p, Node *node = nullptr, int x = 0, int y = -1) {
        if (node == nullptr) node = root, y = size;
        if (x == p && y == p + 1)  return node->data;
        if (x <= p && p < (x + y) / 2) return _setter(node->data, query(p, node->l_child(), x, (x + y) / 2));
        else return _setter(node->data, query(p, node->r_child(), (x + y) / 2, y));
    }
};

struct PersistentSegtree {
    struct Node {
        int l, r, val;
        Node() { l = r = val = 0; }
    };
    int node_size, query_size; int curr;
    vector<int> root; vector<Node> seg;

    PersistentSegtree(int n, int q) {
        node_size = n, query_size = q;
        seg.resize(2 * (n + q * (log2(n) + 1)));
        root = vector<int>(query_size + 10);
        curr = 1, seg[curr].l = seg[curr].r = seg[curr].val = 0;
    }
    int _new_node(int val, int l, int r) {
        seg[curr].val = val, seg[curr].l = l, seg[curr].r = r;
        return curr++;
    }
    int insert(int cur, int idx, int val, int lo, int hi) {
        if (idx < lo || idx > hi)
            return cur;
        else if (lo == hi)
            return _new_node(val, 0, 0);
        int mid = (lo + hi) >> 1;
        int pos = _new_node(-1, insert(seg[cur].l, idx, val, lo, mid),
                            insert(seg[cur].r, idx, val, mid + 1, hi));
        seg[pos].val = max(seg[seg[pos].l].val, seg[seg[pos].r].val);
        return pos;
    }
};