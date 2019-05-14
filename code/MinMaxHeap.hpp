#include <vector>
#include <functional>
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