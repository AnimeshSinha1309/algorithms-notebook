/**
 * @Problem
 * For any (l,r), let S = (((a & b) ^ (a | b)) & (a ^ b)), where a is minimum
 * and b is second minimum. Print the maximum possible value of S for any (l,r)
 * @Reference
 * https://www.hackerrank.com/challenges/and-xor-or/problem
 * segtree, brackets
 */

#include <cmath>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

template <class Type>
class SegmentTree {
   private:
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

class Node {
   public:
    enum Type { OPEN = '(', CLOSE = ')', EXPRESSION = '.' };
    long long total, open, close;
    explicit Node() {
        total = open = close = 0;
    }
    explicit Node(Type bracket) {
        total = 0;
        if (bracket == OPEN) {
            open = 1;
            close = 0;
        } else if (bracket == CLOSE) {
            open = 0;
            close = 1;
        } else {
            open = 0;
            close = 0;
        }
    }
    static Node merge(Node a, Node b) {
        long long temp = min(a.open, b.close);
        Node c(EXPRESSION);
        c.total = a.total + b.total + temp;
        c.open = a.open + b.open - temp;
        c.close = a.close + b.close - temp;
        return c;
    }
};

int main() {
    string st;
    cin >> st;
    vector<Node> list;
    for (char ch : st)
        list.emplace_back(ch == '(' ? Node::OPEN : Node::CLOSE);
    SegmentTree<Node> tree(list, Node::merge, *(new Node));
    int q;
    cin >> q;
    while (q--) {
        unsigned long l, r;
        cin >> l >> r;
        cout << tree.query(l - 1, r).total * 2 << endl;
    }
}