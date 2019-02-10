#ifndef CODE_HEAP_H
#define CODE_HEAP_H


#include <iostream>
#include <vector>
#include <cstdint>
#include "BinaryTreeArray.h"

using namespace std;


class BinaryMinHeap : public BinaryTreeArray {
protected:
    void siftDown(unsigned long pos);
    void siftUp(unsigned long pos);
public:
    explicit BinaryMinHeap(const vector<int> &list);
    BinaryMinHeap(const BinaryMinHeap &heap);
    void insert(int val);
    void remove(unsigned long pos);
    int min();
    vector<int> sort();
};


#endif //CODE_HEAP_H
