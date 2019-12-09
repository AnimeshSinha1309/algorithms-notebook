#ifndef CODE_SEGTREE_H
#define CODE_SEGTREE_H

#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

template <class Type>
class SegmentTree {
   protected:
    vector<Type> data;
    unsigned long size;
    inline unsigned long parent(unsigned long i) {
        return i >> 1;
    }
    inline unsigned long lChild(unsigned long i) {
        return i << 1;
    }
    inline unsigned long rChild(unsigned long i) {
        return i << 1 | 1;
    }
    inline unsigned long sibling(unsigned long i) {
        return i ^ 1;
    }
    inline unsigned long element(unsigned long i) {
        return i + size;
    }
    inline bool isRoot(unsigned long i) {
        return i == 1;
    }
    inline bool islChild(unsigned long i) {
        return (i & 1) == 0;
    }
    inline bool isrChild(unsigned long i) {
        return (i & 1) != 0;
    }
    function<Type(Type, Type)> operation;
    Type defaultValue;

   public:
    explicit SegmentTree(const vector<Type> &list,
                         function<Type(Type, Type)> segOperation,
                         Type defaultTo) {
        size = (1ul << (long)ceil(log2(list.size())));
        data = vector<Type>(size * 2, defaultTo);
        defaultValue = defaultTo;
        operation = segOperation;
        for (unsigned long i = 0; i < list.size(); i++)
            data[i + size] = list[i];
        for (unsigned long i = size - 1; i > 0; --i)
            data[i] = operation(data[lChild(i)], data[rChild(i)]);
    }
    void modify(unsigned long position, Type value) {
        data[element(position)] = value;
        for (data[position = element(position)]; !isRoot(position);
             position = parent(position)) {
            if (islChild(position))
                data[parent(position)] =
                    operation(data[position], data[sibling(position)]);
            if (isrChild(position))
                data[parent(position)] =
                    operation(data[sibling(position)], data[position]);
        }
    }
    Type query(unsigned long l, unsigned long r) {
        Type lAccumulator = defaultValue, rAccumulator = defaultValue;
        for (l = element(l), r = element(r); l < r;
             l = parent(l), r = parent(r)) {
            if (isrChild(l)) {
                lAccumulator = operation(lAccumulator, data[l++]);
            }
            if (isrChild(r)) {
                rAccumulator = operation(data[--r], rAccumulator);
            }
        }
        return operation(lAccumulator, rAccumulator);
    }
};

template <class Type>
class SegmentUpdate {
   protected:
    vector<Type> data;
    unsigned long size;
    inline unsigned long parent(unsigned long i) {
        return i >> 1;
    }
    inline unsigned long lChild(unsigned long i) {
        return i << 1;
    }
    inline unsigned long rChild(unsigned long i) {
        return i << 1 | 1;
    }
    inline unsigned long sibling(unsigned long i) {
        return i ^ 1;
    }
    inline unsigned long element(unsigned long i) {
        return i + size;
    }
    inline bool isRoot(unsigned long i) {
        return i == 1;
    }
    inline bool islChild(unsigned long i) {
        return (i & 1) == 0;
    }
    inline bool isrChild(unsigned long i) {
        return (i & 1) != 0;
    }
    function<Type(Type, Type)> operation;
    Type defaultValue;

   public:
    explicit SegmentUpdate(const vector<Type> &list,
                           function<Type(Type, Type)> segOperation,
                           Type defaultTo) {
        size = (1ul << (long)ceil(log2(list.size())));
        data = vector<Type>(size * 2, defaultTo);
        defaultValue = defaultTo;
        operation = segOperation;
        for (unsigned long i = 0; i < list.size(); i++)
            data[i + size] = list[i];
        for (unsigned long i = size - 1; i > 0; --i)
            data[i] = operation(data[lChild(i)], data[rChild(i)]);
    }
    void modify(unsigned long l, unsigned long r, Type value) {
        for (l = element(l), r = element(r); l < r;
             l = parent(l), r = parent(r)) {
            if (isrChild(l)) {
                data[l] = operation(data[l], value);
                l++;
            }
            if (isrChild(r)) {
                --r;
                data[r] = operation(data[r], value);
            }
        }
    }
    Type query(unsigned long position) {
        Type accumulator = defaultValue;
        for (position = element(position);; position = parent(position)) {
            accumulator = operation(accumulator, data[position]);
            if (isRoot(position))
                break;
        }
        return accumulator;
    }
};

template <typename Type>
class LazySegtree {
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
    void merge(int node) {
        tree[node] = _operation(tree[2 * node], tree[2 * node + 1]);
    }

   public:
    LazySegtree(int n, const function<Type(Type, Type)> &op,
                const function<Type(Type, Type)> &set, const Type identity) {
        for (size = 1; size < n; size <<= 1)
            ;
        _setter = set, _operation = op, _default = identity;
        tree.assign(2 * size, _default);
        lazy.assign(2 * size, _default);
    }

    void modify(int l, int r, Type delta, int node = 1, int x = 0, int y = -1) {
        if (y == -1)
            y = size;
        if (r <= x || l >= y)
            return;
        if (l <= x && y <= r) {
            lazy[node] = _setter(lazy[node], delta);
            tree[node] = _setter(tree[node], delta);
            return;
        }
        split(node);
        modify(l, r, delta, 2 * node, x, (x + y) / 2);
        modify(l, r, delta, 2 * node + 1, (x + y) / 2, y);
        merge(node);
    }
    Type query(int l, int r, int node = 1, int x = 0, int y = -1) {
        if (y == -1)
            y = size;
        if (r <= x || l >= y)
            return _default;
        if (l <= x && y <= r) {
            return tree[node];
        }
        split(node);
        Type lres = query(l, r, 2 * node, x, (x + y) / 2);
        Type rres = query(l, r, 2 * node + 1, (x + y) / 2, y);
        merge(node);
        return _operation(lres, rres);
    }
};

template <typename Type>
class ImplicitSegtree {
    struct Node {
        Type data = 0, lazy = 0;
        Node *l_ptr = nullptr, *r_ptr = nullptr;
        Node *l_child() {
            if (l_ptr == nullptr) {
                l_ptr = new Node;
                r_ptr = new Node;
            }
            return l_ptr;
        }
        Node *r_child() {
            if (r_ptr == nullptr) {
                l_ptr = new Node;
                r_ptr = new Node;
            }
            return r_ptr;
        }
    };
    int size;
    Node *root;
    Type _default;
    function<Type(Type, Type)> _operation;
    function<Type(Type, Type)> _setter;

    void split(Node *node) {
        node->l_child()->lazy = _setter(node->l_child()->lazy, node->lazy);
        node->r_child()->lazy = _setter(node->r_child()->lazy, node->lazy);
        node->l_child()->data = _setter(node->l_child()->data, node->lazy);
        node->r_child()->data = _setter(node->r_child()->data, node->lazy);
        node->lazy = _default;
    }
    void merge(Node *node) {
        node->data = _operation(node->l_child()->data, node->r_child()->data);
    }

   public:
    ImplicitSegtree(int n, const function<Type(Type, Type)> &op,
                    const function<Type(Type, Type)> &set,
                    const Type identity) {
        for (size = 1; size < n; size <<= 1)
            ;
        _setter = set, _operation = op, _default = identity;
        root = new Node;
    }

    void modify(int l, int r, Type delta, Node *node = nullptr, int x = 0,
                int y = -1) {
        if (node == nullptr)
            node = root, y = size;
        if (r <= x || l >= y)
            return;
        if (l <= x && y <= r) {
            node->lazy = _setter(node->lazy, delta);
            node->data = _setter(node->data, delta);
            return;
        }
        split(node);
        modify(l, r, delta, node->l_child(), x, (x + y) / 2);
        modify(l, r, delta, node->r_child(), (x + y) / 2, y);
        merge(node);
    }
    Type query(int l, int r, Node *node = nullptr, int x = 0, int y = -1) {
        if (node == nullptr)
            node = root, y = size;
        if (r <= x || l >= y)
            return _default;
        if (l <= x && y <= r) {
            return node->data;
        }
        split(node);
        Type lres = query(l, r, node->l_child(), x, (x + y) / 2);
        Type rres = query(l, r, node->r_child(), (x + y) / 2, y);
        merge(node);
        return _operation(lres, rres);
    }
};

template <typename Type>
class ImplicitSegupdate {
    struct Node {
        Type data = 0;
        Node *l_ptr = nullptr, *r_ptr = nullptr;
        Node *l_child() {
            if (l_ptr == nullptr) {
                l_ptr = new Node;
                r_ptr = new Node;
            }
            return l_ptr;
        }
        Node *r_child() {
            if (r_ptr == nullptr) {
                l_ptr = new Node;
                r_ptr = new Node;
            }
            return r_ptr;
        }
    };
    int size;
    Node *root;
    function<Type(Type, Type)> _setter;

   public:
    ImplicitSegupdate(int n, const function<Type(Type, Type)> &set) {
        for (size = 1; size < n; size <<= 1)
            ;
        _setter = set;
        root = new Node;
    }

    void modify(int l, int r, Type delta, Node *node = nullptr, int x = 0,
                int y = -1) {
        if (node == nullptr)
            node = root, y = size;
        if (r <= x || l >= y)
            return;
        if (l <= x && y <= r) {
            node->data = _setter(node->data, delta);
            return;
        }
        modify(l, r, delta, node->l_child(), x, (x + y) / 2);
        modify(l, r, delta, node->r_child(), (x + y) / 2, y);
    }
    Type query(int p, Node *node = nullptr, int x = 0, int y = -1) {
        if (node == nullptr)
            node = root, y = size;
        if (x == p && y == p + 1) {
            return node->data;
        }
        0 if (x <= p && p < (x + y) / 2) return _setter(
            node->data, query(p, node->l_child(), x, (x + y) / 2));
        else return _setter(node->data,
                            query(p, node->r_child(), (x + y) / 2, y));
    }
};

#endif  // CODE_SEGTREE_H
