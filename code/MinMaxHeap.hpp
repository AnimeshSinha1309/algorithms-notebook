#ifndef CODE_MINMAXHEAP_H
#define CODE_MINMAXHEAP_H

#include <iostream>
#include <cmath>
#include <functional>
#include <vector>

using namespace std;

/*
 * A container for MinMaxHeap
 * 
 * Based on the original 1986 paper
 *    https://cglab.ca/~morin/teaching/5408/refs/minmax.pdf
 */
class MinMaxHeap
{
private:
    vector<long long> heap;
    bool isMinLevel(long long index);
    void siftDown(long long index);
    void siftDownMin(long long index);
    void siftDownMax(long long index);
    void siftDownGeneric(long long index, function<bool(long long, long long)> comp);
    void siftUp(long long index);
    void siftUpMin(long long index);
    void siftUpMax(long long index);
    void siftUpByGrandParentGeneric(long long index, function<bool(long long, long long)> comp);
    void swap(long long a, long long b);

public:
    MinMaxHeap();
    MinMaxHeap(vector<long long> base);
    void insert(long long n);
    long long getMax();
    long long getMin();
    long long size();
    bool empty();
    long long popMin();
    long long popMax();
    vector<long long> heapify(vector<long long> base);
    static bool minHeapComparator(long long a, long long b);
    static bool maxHeapComparator(long long a, long long b);
    void print();
};

MinMaxHeap::MinMaxHeap()
{
    heap = vector<long long>(0);
    heap.push_back(0); // push in a dummy element
}

// create new object based on the given array
// using heapify method
MinMaxHeap::MinMaxHeap(vector<long long> base)
{
    heap = MinMaxHeap::heapify(base);
}

bool MinMaxHeap::isMinLevel(long long index)
{
    return ((long long)floor(log2(index)) + 1) & 1;
}

bool MinMaxHeap::minHeapComparator(long long parent, long long child)
{
    return parent < child;
}

bool MinMaxHeap::maxHeapComparator(long long parent, long long child)
{
    return parent > child;
}

void MinMaxHeap::siftDown(long long index)
{
    if (isMinLevel(index))
    {
        siftDownMin(index);
    }
    else
    {
        siftDownMax(index);
    }
}

void MinMaxHeap::swap(long long a, long long b)
{
    long long t = heap[a];
    heap[a] = heap[b];
    heap[b] = t;
}

void MinMaxHeap::siftDownMin(long long index)
{
    siftDownGeneric(index, minHeapComparator);
}

void MinMaxHeap::siftDownMax(long long index)
{
    siftDownGeneric(index, maxHeapComparator);
}

void MinMaxHeap::siftDownGeneric(long long index, function<bool(long long, long long)> comp)
{
    long long m = index;
    // find smallest node among children and grandchildren
    vector<long long> indicesToCheck = {2 * index, 2 * index + 1, 4 * index, 4 * index + 1, 4 * index + 2, 4 * index + 3};
    for (auto idx : indicesToCheck)
    {
        if (idx <= (long long)heap.size() - 1 && !comp(heap[m], heap[idx]))
            m = idx;
    }
    // curr node itself is smallest
    if (m == index)
        return;
    // bring smallest node of subtree to current pos
    swap(m, index);
    // when smallest node is a grandchild
    if (m > 2 * index + 1)
    {
        // check additional case for even level
        if (m > 1 && !comp(heap[m], heap[m / 2]))
            swap(m, m / 2);
        // recursively go ahead
        siftDownGeneric(m, comp);
    }
}

void MinMaxHeap::print()
{
    for (auto x : heap)
    {
        cout << x << " ";
    }
    cout << "\n";
}

void MinMaxHeap::siftUp(long long index)
{
    // cannot siftup root
    if (index == 1)
        return;
    if (isMinLevel(index))
    {
        if (!maxHeapComparator(heap[index / 2], heap[index]))
        {
            swap(index, index / 2);
            siftUpMax(index / 2);
        }
        else
        {
            siftUpMin(index);
        }
    }
    else
    {
        if (!minHeapComparator(heap[index / 2], heap[index]))
        {
            swap(index, index / 2);
            siftUpMin(index / 2);
        }
        else
        {
            siftUpMax(index);
        }
    }
}

void MinMaxHeap::siftUpMin(long long index)
{
    siftUpByGrandParentGeneric(index, minHeapComparator);
}

void MinMaxHeap::siftUpMax(long long index)
{
    siftUpByGrandParentGeneric(index, maxHeapComparator);
}

void MinMaxHeap::siftUpByGrandParentGeneric(long long index, function<bool(long long, long long)> comp)
{
    // does not have grandparent
    long long gp = index / 4;
    if (gp == 0)
        return;
    if (!comp(heap[gp], heap[index]))
    {
        swap(gp, index);
        siftUpMin(gp);
    }
}

// O(log n)
void MinMaxHeap::insert(long long n)
{
    heap.push_back(n);
    siftUp(heap.size() - 1);
}

bool MinMaxHeap::empty()
{
    return heap.size() == 1;
}

// O(1)
long long MinMaxHeap::getMax()
{
    if (empty())
    {
        return -1;
    }
    return heap.size() > 3 ? max(heap[3], heap[2]) : heap.size() > 2 ? heap[2] : heap[1];
}

// O(1)
long long MinMaxHeap::getMin()
{
    if (empty())
    {
        return -1;
    }
    return heap[1];
}

long long MinMaxHeap::popMin()
{
    if (empty())
    {
        return -1;
    }
    long long mi = getMin();
    swap(1, heap.size() - 1);
    heap.pop_back();
    print();
    siftDown(1);
    print();
    return mi;
}

long long MinMaxHeap::popMax()
{
    if (empty())
    {
        return -1;
    }

    long long ma = getMax();
    if (heap.size() == 2 || heap.size() == 3)
    {
        heap.pop_back();
        return ma;
    }
    else
    {
        long long maIndex = 2;
        if (heap[3] > heap[2])
            maIndex = 3;
        swap(maIndex, heap.size() - 1);
        heap.pop_back();
        siftDown(maIndex);
        return ma;
    }
}

long long MinMaxHeap::size()
{
    return heap.size() - 1;
}

/*
 * Parameters
 * vector<long long> base: a one-based array that needs to be heapified
 */
vector<long long> MinMaxHeap::heapify(vector<long long> base)
{
    long long n = base.size();
    vector<long long> heap(base.begin(), base.end()); // work on a copy

    for (long long i = n; i >= 1; i--)
    {
        siftDown(i);
    }

    return heap;
}

#endif