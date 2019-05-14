#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include "MinMaxHeap.hpp"

typedef long long int ll;
using namespace std;

MinMaxHeap::MinMaxHeap()
{
    heap = vector<ll>(0);
    heap.push_back(0); // push in a dummy element
}

// create new object based on the given array
// using heapify method
MinMaxHeap::MinMaxHeap(vector<ll> base)
{
    heap = MinMaxHeap::heapify(base);
}

bool MinMaxHeap::isMinLevel(ll index)
{
    return ((ll)floor(log2(index)) + 1) & 1;
}

bool MinMaxHeap::minHeapComparator(ll parent, ll child)
{
    return parent < child;
}

bool MinMaxHeap::maxHeapComparator(ll parent, ll child)
{
    return parent > child;
}

void MinMaxHeap::siftDown(ll index)
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

void MinMaxHeap::swap(ll a, ll b)
{
    ll t = heap[a];
    heap[a] = heap[b];
    heap[b] = t;
}

void MinMaxHeap::siftDownMin(ll index)
{
    siftDownGeneric(index, minHeapComparator);
}

void MinMaxHeap::siftDownMax(ll index)
{
    siftDownGeneric(index, maxHeapComparator);
}

void MinMaxHeap::siftDownGeneric(ll index, function<bool(ll, ll)> comp)
{
    ll m = index;

    // find smallest node among children and grandchildren
    vector<ll> indicesToCheck = {2 * index, 2 * index + 1, 4 * index, 4 * index + 1, 4 * index + 2, 4 * index + 3};

    for (auto idx : indicesToCheck)
    {
        if (idx <= (ll)heap.size() - 1 && !comp(heap[m], heap[idx]))
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
        {
            swap(m, m / 2);
        }
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

void MinMaxHeap::siftUp(ll index)
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

void MinMaxHeap::siftUpMin(ll index)
{
    siftUpByGrandParentGeneric(index, minHeapComparator);
}

void MinMaxHeap::siftUpMax(ll index)
{
    siftUpByGrandParentGeneric(index, maxHeapComparator);
}

void MinMaxHeap::siftUpByGrandParentGeneric(ll index, function<bool(ll, ll)> comp)
{
    // does not have grandparent
    ll gp = index / 4;
    if (gp == 0)
        return;

    if (!comp(heap[gp], heap[index]))
    {
        swap(gp, index);
        siftUpMin(gp);
    }
}

// O(log n)
void MinMaxHeap::insert(ll n)
{
    heap.push_back(n);
    siftUp(heap.size() - 1);
}

bool MinMaxHeap::empty()
{
    return heap.size() == 1;
}

// O(1)
ll MinMaxHeap::getMax()
{
    if (empty())
    {
        return -1;
    }
    return heap.size() > 3 ? max(heap[3], heap[2]) : heap.size() > 2 ? heap[2] : heap[1];
}

// O(1)
ll MinMaxHeap::getMin()
{
    if (empty())
    {
        return -1;
    }
    return heap[1];
}

ll MinMaxHeap::popMin()
{
    if (empty())
    {
        return -1;
    }
    ll mi = getMin();
    swap(1, heap.size() - 1);
    heap.pop_back();
    print();
    siftDown(1);
    print();
    return mi;
}

ll MinMaxHeap::popMax()
{
    if (empty())
    {
        return -1;
    }

    ll ma = getMax();
    if (heap.size() == 2 || heap.size() == 3)
    {
        heap.pop_back();
        return ma;
    }
    else
    {
        ll maIndex = 2;
        if (heap[3] > heap[2])
            maIndex = 3;
        swap(maIndex, heap.size() - 1);
        heap.pop_back();
        siftDown(maIndex);
        return ma;
    }
}

ll MinMaxHeap::size()
{
    return heap.size() - 1;
}

/*
 * Parameters
 * vector<ll> base: a one-based array that needs to be heapified
 */
vector<ll> MinMaxHeap::heapify(vector<ll> base)
{
    ll n = base.size();
    vector<ll> heap(base.begin(), base.end()); // work on a copy

    for (ll i = n; i >= 1; i--)
    {
        siftDown(i);
    }

    return heap;
}