#ifndef CODE_HEAP_H
#define CODE_HEAP_H


#include <iostream>
#include <vector>
#include <cstdint>
#include <functional>

using namespace std;

template <class Type>
class BinaryHeap {
protected:
    function<bool (Type, Type)> comparator;

    vector<Type> data;
    void siftDown(unsigned long pos) {
        if (pos >= this->data.size()) return;
        bool lChildExists = this->lChild(pos) < this->data.size(), rChildExists = this->rChild(pos) < this->data.size();
        if ((lChildExists && comparator(this->data[this->lChild(pos)], this->data[pos]))
                || (rChildExists && comparator(this->data[this->rChild(pos)], this->data[pos]))) {
            if (!rChildExists || (lChildExists &&
                    comparator(this->data[this->lChild(pos)], this->data[this->rChild(pos)]))) {
                this->swap(pos, this->lChild(pos));
                siftDown(this->lChild(pos));
            } else {
                this->swap(pos, this->rChild(pos));
                siftDown(this->rChild(pos));
            }
        }
    }
    void siftUp(unsigned long pos) {
        if (pos == 0) return; // This is the Root Element
        if (comparator(this->data[pos], this->data[this->parent(pos)])) {
            this->swap(pos, this->parent(pos));
            this->siftUp(this->parent(pos));
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
        this->data[x] ^= this->data[y];
        this->data[y] ^= this->data[x];
        this->data[x] ^= this->data[y];
    }
public:
    explicit BinaryHeap(const vector<Type> &list, function<Type (Type, Type)> heapComparator = less<>()) {
        this->comparator = heapComparator;
        for (auto i : list) this->data.push_back(i);
        for (long i = this->data.size() - 1; i >= 0; i--) {
            this->siftDown((unsigned) i);
        }
    }
    explicit BinaryHeap(function<Type (Type, Type)> heapComparator = less<>()) {
        this->comparator = heapComparator;
        this->data = vector<Type>();
    }
    BinaryHeap(const BinaryHeap &heap) {
        this->comparator = heap.comparator;
        for (auto val : heap.data) this->data.push_back(val);
    }
    void insert(int val) {
        this->data.push_back(val);
        this->siftUp(this->data.size() - 1);
    }
    void remove(unsigned long pos) {
        this->swap(this->data.size() - 1, pos);
        this->data.pop_back();
        this->siftUp(pos);
        this->siftDown(pos);
    }
    Type top() {
        return this->data[0];
    }
    vector<Type> sort() {
        vector<Type> result;
        BinaryHeap copy(*this);
        while(!copy.data.empty()) {
            result.push_back(copy.top());
            copy.remove(0);
        }
        return result;
    }
};


#endif //CODE_HEAP_H
