#include <iostream>
#include <vector>
#include "BinaryMinHeap.h"
using namespace std;

int main() {
    vector<int> data({5, 7, 2, 9, 12, 0, 12, 34, 1, 23, 45, 9, 16});
    BinaryMinHeap heap(data); heap.insert(4); heap.print();
    vector<int> sorted = heap.sort();
    for (auto item : sorted) cout << item << " "; cout << endl;
}