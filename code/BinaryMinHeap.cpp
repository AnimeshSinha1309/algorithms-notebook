#include "BinaryMinHeap.h"


BinaryMinHeap::BinaryMinHeap(const vector<int> &list) : BinaryTreeArray(list) {
    for (long i = this->data.size() - 1; i >= 0; i--) {
        this->siftDown((unsigned) i);
    }
}

BinaryMinHeap::BinaryMinHeap(const BinaryMinHeap &heap) : BinaryTreeArray(heap) {
    ;
}

void BinaryMinHeap::siftDown(unsigned long pos) {
    // Handling cases where Element does not exist / Does not have either or both it's children.
    if (pos >= this->data.size()) return;
    int lCh = INT32_MAX, rCh = INT32_MAX;
    if (this->lChild(pos) < this->data.size()) lCh = this->data[this->lChild(pos)];
    if (this->rChild(pos) < this->data.size()) rCh = this->data[this->rChild(pos)];
    // Recursive Sifting down and Heapification
    if (lCh < this->data[pos] || rCh < this->data[pos]) {
        if (lCh < rCh) {
            this->swap(pos, this->lChild(pos));
            siftDown(this->lChild(pos));
        } else {
            this->swap(pos, this->rChild(pos));
            siftDown(this->rChild(pos));
        }
    }
}

void BinaryMinHeap::siftUp(unsigned long pos) {
    if (pos == 0) return; // This is the Root Element
    if (this->data[this->parent(pos)] > this->data[pos]) {
        this->swap(pos, this->parent(pos));
        this->siftUp(this->parent(pos));
    }
}

void BinaryMinHeap::insert(int val) {
    this->data.push_back(val);
    this->siftUp(this->data.size() - 1);
}

void BinaryMinHeap::remove(unsigned long pos) {
    this->swap(this->data.size() - 1, pos);
    this->data.pop_back();
    this->siftUp(pos);
    this->siftDown(pos);
}

vector<int> BinaryMinHeap::sort() {
    vector<int> result;
    BinaryMinHeap copy(*this);
    while(!copy.data.empty()) {
        result.push_back(copy.min());
        copy.remove(0);
    }
    return result;
}

int BinaryMinHeap::min() {
    return this->data[0];
}