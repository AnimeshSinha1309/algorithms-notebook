#include <iostream>
#include "BinaryHeap.h"
#include "SegmentTree.h"

using namespace std;

int main() {
    BinaryHeap<int> heap({7, 5, 3, 9, 1, 6}, greater<>());
    cout << "Binary Heap Sort: "; for (auto el : heap.sort()) cout << el << " "; cout << endl;
    SegmentTree<int> segtree({2, 3, 8, 5, 6, 9}, [](int x, int y) { return x + y; }, 0);
    cout << "Segment Tree Sum: " << segtree.query(2, 4) << endl;
}