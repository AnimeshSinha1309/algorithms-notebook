#ifndef CODE_BINARYTREE_H
#define CODE_BINARYTREE_H


#include <iostream>
#include <vector>
using namespace std;

class BinaryTreeArray {
protected:
    vector<int> data;

    unsigned long parent(unsigned long val);
    unsigned long lChild(unsigned long val);
    unsigned long rChild(unsigned long val);
    void swap(unsigned long p1, unsigned long p2);
public:
    explicit BinaryTreeArray();
    explicit BinaryTreeArray(vector<int> list);
    BinaryTreeArray(const BinaryTreeArray &tree);
    void print();
    unsigned long size();
};


#endif //CODE_BINARYTREE_H
