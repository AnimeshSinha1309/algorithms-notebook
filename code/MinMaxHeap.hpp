#ifndef CODE_MINMAXHEAP_H
#define CODE_MINMAXHEAP_H

#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

/*
 * A container for MinMaxHeap
 *
 * Based on the original 1986 paper
 *    https://cglab.ca/~morin/teaching/5408/refs/minmax.pdf
 */
class MinMaxHeap {
   private:
    vector<long long> heap;
    bool _is_min_level(long long index);
    void _sift_down(long long index);
    void _sift_down_min(long long index);
    void _sift_down_max(long long index);
    void _sift_down_generic(long long index,
                            function<bool(long long, long long)> comp);
    void _sift_up(long long index);
    void _sift_up_min(long long index);
    void _sift_up_max(long long index);
    void _sift_up_grandparent_generic(
        long long index, function<bool(long long, long long)> comp);
    void swap(long long a, long long b);

   public:
    MinMaxHeap();
    MinMaxHeap(vector<long long> base);
    void insert(long long n);
    long long get_max();
    long long get_min();
    long long size();
    bool empty();
    long long pop_min();
    long long pop_max();
    vector<long long> heapify(vector<long long> base);
    static bool _min_heap_comparator(long long a, long long b);
    static bool _max_heap_comparator(long long a, long long b);
    void print();
};

MinMaxHeap::MinMaxHeap() {
    heap = vector<long long>(0);
    heap.push_back(0);  // push in a dummy element
}

// create new object based on the given array
// using heapify method
MinMaxHeap::MinMaxHeap(vector<long long> base) {
    heap = MinMaxHeap::heapify(base);
}

bool MinMaxHeap::_is_min_level(long long index) {
    return ((long long)floor(log2(index)) + 1) & 1;
}

bool MinMaxHeap::_min_heap_comparator(long long parent, long long child) {
    return parent < child;
}

bool MinMaxHeap::_max_heap_comparator(long long parent, long long child) {
    return parent > child;
}

void MinMaxHeap::_sift_down(long long index) {
    if (_is_min_level(index)) {
        _sift_down_min(index);
    } else {
        _sift_down_max(index);
    }
}

void MinMaxHeap::swap(long long a, long long b) {
    long long t = heap[a];
    heap[a] = heap[b];
    heap[b] = t;
}

void MinMaxHeap::_sift_down_min(long long index) {
    _sift_down_generic(index, _min_heap_comparator);
}

void MinMaxHeap::_sift_down_max(long long index) {
    _sift_down_generic(index, _max_heap_comparator);
}

void MinMaxHeap::_sift_down_generic(long long index,
                                    function<bool(long long, long long)> comp) {
    long long m = index;
    // find smallest node among children and grandchildren
    vector<long long> indicesToCheck = {2 * index,     2 * index + 1,
                                        4 * index,     4 * index + 1,
                                        4 * index + 2, 4 * index + 3};
    for (auto idx : indicesToCheck) {
        if (idx <= (long long)heap.size() - 1 && !comp(heap[m], heap[idx]))
            m = idx;
    }
    // curr node itself is smallest
    if (m == index)
        return;
    // bring smallest node of subtree to current pos
    swap(m, index);
    // when smallest node is a grandchild
    if (m > 2 * index + 1) {
        // check additional case for even level
        if (m > 1 && !comp(heap[m], heap[m / 2]))
            swap(m, m / 2);
        // recursively go ahead
        _sift_down_generic(m, comp);
    }
}

void MinMaxHeap::print() {
    for (auto x : heap) {
        cout << x << " ";
    }
    cout << "\n";
}

void MinMaxHeap::_sift_up(long long index) {
    // cannot siftup root
    if (index == 1)
        return;
    if (_is_min_level(index)) {
        if (!_max_heap_comparator(heap[index / 2], heap[index])) {
            swap(index, index / 2);
            _sift_up_max(index / 2);
        } else {
            _sift_up_min(index);
        }
    } else {
        if (!_min_heap_comparator(heap[index / 2], heap[index])) {
            swap(index, index / 2);
            _sift_up_min(index / 2);
        } else {
            _sift_up_max(index);
        }
    }
}

void MinMaxHeap::_sift_up_min(long long index) {
    _sift_up_grandparent_generic(index, _min_heap_comparator);
}

void MinMaxHeap::_sift_up_max(long long index) {
    _sift_up_grandparent_generic(index, _max_heap_comparator);
}

void MinMaxHeap::_sift_up_grandparent_generic(
    long long index, function<bool(long long, long long)> comp) {
    // does not have grandparent
    long long gp = index / 4;
    if (gp == 0)
        return;
    if (!comp(heap[gp], heap[index])) {
        swap(gp, index);
        _sift_up_min(gp);
    }
}

// O(log n)
void MinMaxHeap::insert(long long n) {
    heap.push_back(n);
    _sift_up(heap.size() - 1);
}

bool MinMaxHeap::empty() {
    return heap.size() == 1;
}

// O(1)
long long MinMaxHeap::get_max() {
    if (empty()) {
        return -1;
    }
    return heap.size() > 3 ? max(heap[3], heap[2])
                           : heap.size() > 2 ? heap[2] : heap[1];
}

// O(1)
long long MinMaxHeap::get_min() {
    if (empty()) {
        return -1;
    }
    return heap[1];
}

long long MinMaxHeap::pop_min() {
    if (empty()) {
        return -1;
    }
    long long mi = get_min();
    swap(1, heap.size() - 1);
    heap.pop_back();
    print();
    _sift_down(1);
    print();
    return mi;
}

long long MinMaxHeap::pop_max() {
    if (empty()) {
        return -1;
    }

    long long ma = get_max();
    if (heap.size() == 2 || heap.size() == 3) {
        heap.pop_back();
        return ma;
    } else {
        long long maIndex = 2;
        if (heap[3] > heap[2])
            maIndex = 3;
        swap(maIndex, heap.size() - 1);
        heap.pop_back();
        _sift_down(maIndex);
        return ma;
    }
}

long long MinMaxHeap::size() {
    return heap.size() - 1;
}

/*
 * Parameters
 * vector<long long> base: a one-based array that needs to be heapified
 */
vector<long long> MinMaxHeap::heapify(vector<long long> base) {
    long long n = base.size();
    vector<long long> heap(base.begin(), base.end());  // work on a copy

    for (long long i = n; i >= 1; i--) {
        _sift_down(i);
    }

    return heap;
}

#endif