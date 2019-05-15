#ifndef CODE_SEGTREE_H
#define CODE_SEGTREE_H


#include <iostream>
#include <vector>
#include <cmath>
#include <functional>

using namespace std;

template<class Type>
class SegmentTree {
protected:
    vector<Type> data; unsigned long size;
    inline unsigned long parent(unsigned long i) { return i >> 1; }
    inline unsigned long lChild(unsigned long i) { return i << 1; }
    inline unsigned long rChild(unsigned long i) { return i << 1 | 1; }
    inline unsigned long sibling(unsigned long i) { return i ^ 1; }
    inline unsigned long element(unsigned long i) { return i + size; }
    inline bool isRoot(unsigned long i) { return i == 1; }
    inline bool islChild(unsigned long i) { return (i & 1) == 0; }
    inline bool isrChild(unsigned long i) { return (i & 1) != 0; }
    function<Type (Type, Type)> operation;
    Type defaultValue;
public:
    explicit SegmentTree(const vector<Type> &list, function<Type (Type, Type)> segOperation, Type defaultTo) {
        size = (1ul << (long)ceil(log2(list.size())));
        data = vector<Type>(size * 2, defaultTo);
        defaultValue = defaultTo; operation = segOperation;
        for (unsigned long i = 0; i < list.size(); i++) data[i + size] = list[i];
        for (unsigned long i = size - 1; i > 0; --i) data[i] = operation(data[lChild(i)], data[rChild(i)]);
    }
    void modify(unsigned long position, Type value) {
        data[element(position)] = value;
        for (data[position = element(position)]; !isRoot(position); position = parent(position)) {
            if (islChild(position)) data[parent(position)] = operation(data[position], data[sibling(position)]);
            if (isrChild(position)) data[parent(position)] = operation(data[sibling(position)], data[position]);
        }
    }
    Type query(unsigned long l, unsigned long r) {
        Type lAccumulator = defaultValue, rAccumulator = defaultValue;
        for (l = element(l), r = element(r); l < r; l = parent(l), r = parent(r)) {
            if (isrChild(l)) { lAccumulator = operation(lAccumulator, data[l++]); }
            if (isrChild(r)) { rAccumulator = operation(data[--r], rAccumulator); }
        } return operation(lAccumulator, rAccumulator);
    }
};


#endif //CODE_HEAP_H
