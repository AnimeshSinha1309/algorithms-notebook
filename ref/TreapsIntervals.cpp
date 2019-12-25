#include "template.hpp"

struct Treap {
    struct Node {
        int val, prior, size;
        Node *l, *r;
    };
    Node* root;

    int size(Node* node) {
        return node ? node->size : 0;
    }

    void split(Node* t, Node*& l, Node*& r, int key) {
        if (!t)
            l = r = NULL;
        else if (t->val <= key)
            split(t->r, t->r, r, key), l = t;
        else
            split(t->l, l, t->l, key), r = t;
        if (t)
            t->size = size(t->l) + 1 + size(t->r);
    }
    void merge(Node*& t, Node* l, Node* r) {
        if (!l || !r)
            t = l ? l : r;
        else if (l->prior > r->prior)
            merge(l->r, l->r, r), t = l;
        else
            merge(r->l, l, r->l), t = r;
        if (t)
            t->size = size(t->l) + 1 + size(t->r);
    }
    void insert(Node*& root, Node* it) {
        if (!root)
            root = it;
        else if (it->prior > root->prior)
            split(root, it->l, it->r, it->val), root = it;
        else
            insert(root->val < it->val ? root->r : root->l, it);
        if (root)
            root->size = size(root->l) + 1 + size(root->r);
    }
    void erase(Node*& root, int key) {
        if (!root)
            return;
        else if (root->val == key) {
            Node* x = root;
            merge(root, root->l, root->r);
            free(x);
        } else
            erase(root->val < key ? root->r : root->l, key);
        if (root)
            root->size = size(root->l) + 1 + size(root->r);
    }
    void unite(Node*& root, Node* l, Node* r) {
        if (!l || !r)
            return void(root = l ? l : r);
        Node *lt, *rt;
        if (l->prior < r->prior)
            swap(l, r);
        split(r, lt, rt, l->val);
        unite(l->l, l->l, lt);
        unite(l->r, l->r, rt);
        root = l;
        if (root)
            root->size = size(root->l) + 1 + size(root->r);
    }
    Node* _create_node(int val) {
        Node* ret = (Node*)malloc(sizeof(Node));
        ret->val = val;
        ret->size = 1;
        ret->prior = rand();
        ret->l = ret->r = NULL;
        return ret;
    }
    // insert(_create_node(x), head);
};

struct IntervalTreap : Treap {
    struct Node {
        int prior, size, val, sum, lazy;
        Node *l, *r;
    };
    int sz(Node* t) {
        return t ? t->size : 0;
    }
    void upd_sz(Node* t) {
        if (t)
            t->size = sz(t->l) + 1 + sz(t->r);
    }
    void lazy(Node* t) {
        if (!t || !t->lazy)
            return;
        t->val += t->lazy;
        t->sum += t->lazy * sz(t);
        if (t->l)
            t->l->lazy += t->lazy;
        if (t->r)
            t->r->lazy += t->lazy;
        t->lazy = 0;
    }
    void reset(Node* t) {
        if (t)
            t->sum = t->val;
    }
    void combine(Node*& t, Node* l, Node* r) {
        if (!l || !r)
            return void(t = l ? l : r);
        t->sum = l->sum + r->sum;
    }
    void operation(Node* t) {
        if (!t)
            return;
        reset(t);
        lazy(t->l);
        lazy(t->r);
        combine(t, t->l, t);
        combine(t, t, t->r);
    }
    void split(Node* t, Node*& l, Node*& r, int pos, int add = 0) {
        if (!t)
            return void(l = r = NULL);
        lazy(t);
        int cpos = add + sz(t->l);
        if (cpos <= pos)
            split(t->r, t->r, r, pos, cpos + 1), l = t;
        else
            split(t->l, l, t->l, pos, add), r = t;
        upd_sz(t);
        operation(t);
    }
    void merge(Node*& t, Node* l, Node* r) {
        lazy(l);
        lazy(r);
        if (!l || !r)
            t = l ? l : r;
        else if (l->prior > r->prior)
            merge(l->r, l->r, r), t = l;
        else
            merge(r->l, l, r->l), t = r;
        upd_sz(t);
        operation(t);
    }
    Node* init(int val) {
        Node* ret = (Node*)malloc(sizeof(Node));
        ret->prior = rand();
        ret->size = 1;
        ret->val = val;
        ret->sum = val;
        ret->lazy = 0;
        return ret;
    }
    int range_query(Node* t, int l, int r) {
        Node *left, *mid, *right;
        split(t, left, mid, l - 1);
        split(mid, t, right, r - l); /*note: r-l!*/
        int ans = t->sum;
        merge(mid, left, t);
        merge(t, mid, right);
        return ans;
    }
    void range_update(Node* t, int l, int r, int val) {
        Node *left, *mid, *right;
        split(t, left, mid, l - 1);
        split(mid, t, right, r - l); /*note: r-l!*/
        t->lazy += val;
        merge(mid, left, t);
        merge(t, mid, right);
    }
};