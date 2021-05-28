#ifndef CODE_BINARY_HEAP_H
#define CODE_BINARY_HEAP_H

#include "../ref/template.hpp"

template <class Type>
class BinaryHeap {
   protected:
    function<bool(Type, Type)> comp;

    vector<Type> data;
    void siftDown(unsigned long pos) {
        if (pos >= data.size())
            return;
        bool hasLChild = lChild(pos) < data.size(),
             hasRChild = rChild(pos) < data.size();
        if ((hasLChild && comp(data[lChild(pos)], data[pos])) ||
            (hasRChild && comp(data[rChild(pos)], data[pos]))) {
            if (!hasRChild ||
                (hasLChild && comp(data[lChild(pos)], data[rChild(pos)]))) {
                swap(pos, lChild(pos));
                siftDown(lChild(pos));
            } else {
                swap(pos, rChild(pos));
                siftDown(rChild(pos));
            }
        }
    }
    void siftUp(unsigned long pos) {
        if (pos == 0)
            return;  // This is the Root Element
        if (comp(data[pos], data[parent(pos)])) {
            swap(pos, parent(pos));
            siftUp(parent(pos));
        }
    }
    inline unsigned long parent(unsigned long val) {
        return (val - 1) / 2;
    }
    inline unsigned long lChild(unsigned long val) {
        return 2 * val + 1;
    }
    inline unsigned long rChild(unsigned long val) {
        return 2 * val + 2;
    }
    void swap(unsigned long x, unsigned long y) {
        data[x] ^= data[y];
        data[y] ^= data[x];
        data[x] ^= data[y];
    }

   public:
    explicit BinaryHeap(const vector<Type> &list,
                        function<Type(Type, Type)> heapComparator = less<>()) {
        comp = heapComparator;
        for (auto i : list)
            data.push_back(i);
        for (long i = data.size() - 1; i >= 0; i--) {
            siftDown((unsigned)i);
        }
    }
    explicit BinaryHeap(function<Type(Type, Type)> heapComparator = less<>()) {
        comp = heapComparator;
        data = vector<Type>();
    }
    BinaryHeap(const BinaryHeap &heap) {
        comp = heap.comp;
        for (auto val : heap.data)
            data.push_back(val);
    }
    void insert(int val) {
        data.push_back(val);
        siftUp(data.size() - 1);
    }
    void remove(unsigned long pos) {
        swap(data.size() - 1, pos);
        data.pop_back();
        siftUp(pos);
        siftDown(pos);
    }
    Type top() {
        return data[0];
    }
    vector<Type> sort() {
        vector<Type> result;
        BinaryHeap copy(*this);
        while (!copy.data.empty()) {
            result.push_back(copy.top());
            copy.remove(0);
        }
        return result;
    }
};

#endif  // CODE_BINARY_HEAP_H
