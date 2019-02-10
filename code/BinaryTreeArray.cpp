#include "BinaryTreeArray.h"


BinaryTreeArray::BinaryTreeArray() {
    ;
}
BinaryTreeArray::BinaryTreeArray(vector<int> list) {
    for (int i : list) this->data.push_back(i);
}
BinaryTreeArray::BinaryTreeArray(const BinaryTreeArray &tree) {
    for (auto val : tree.data) this->data.push_back(val);
}
void BinaryTreeArray::print() {
    for (auto val : this->data) cout << val << " "; cout << endl;
}

unsigned long BinaryTreeArray::parent(unsigned long val) { return (val - 1) / 2; }
unsigned long BinaryTreeArray::lChild(unsigned long val) { return 2 * val + 1; }
unsigned long BinaryTreeArray::rChild(unsigned long val) { return 2 * val + 2; }

void BinaryTreeArray::swap(unsigned long p1, unsigned long p2) {
    this->data[p1] ^= this->data[p2];
    this->data[p2] ^= this->data[p1];
    this->data[p1] ^= this->data[p2];
}
unsigned long BinaryTreeArray::size() {
    return this->data.size();
}