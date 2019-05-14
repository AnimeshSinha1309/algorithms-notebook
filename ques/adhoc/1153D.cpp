#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long long leaves;
class Node {
public:
    bool maximizer; unsigned long long rank = 1; vector<Node*> children;
    unsigned long long choice() {
        for (auto child : children) rank = this->maximizer ? min(rank, child->choice()) : rank + child->choice();
        if (children.empty()) leaves += 1; return rank;
    }
};

int main() {
    unsigned int n; cin >> n; vector<Node> tree(n); leaves = 0;
    for (int i = 0; i < n; i++) { int x; cin >> x; tree[i].maximizer = (x == 1); }
    for (int i = 1; i < n; i++) { int x; cin >> x; tree[x-1].children.push_back(&tree[i]); }
    cout << leaves - tree[0].choice() << endl;
}