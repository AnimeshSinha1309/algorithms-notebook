//
// Created by animesh on 11/2/19.
//

#ifndef CODE_BINOMIALTREE_H
#define CODE_BINOMIALTREE_H


#include <vector>
using namespace std;

class BinomialTree {

    class Node {
        Node *parent, *sibling, *children;
        int value;
    };

    BinomialTree(vector<int> list);

};


#endif //CODE_BINOMIALTREE_H
