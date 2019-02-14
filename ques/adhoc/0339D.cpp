/**
 * @Problem
 * For an array of size 2^n, magic number is OR of adjacent positions (1,2), (3,4) ... (2^n-1,2^n).
 * then the XOR of the results, then OR and XOR alternating at each stage till there is only 1
 * element left in the resultant array. Array will have one element edited in each query, print
 * magic number after it.
 * @Reference
 * https://codeforces.com/contest/339/problem/D
 * data structures, trees, *1700
 */

#include <iostream>
#include <vector>
using namespace std;

#define PARENT(i) ((i-1)/2)
#define LCHILD(i) (2*i + 1)
#define RCHILD(i) (2*i + 2)
#define POS(i) (i + (1 << n) - 1)

int main() {
    unsigned int n, m; cin >> n >> m; unsigned int l[(1 << (n+1)) - 1]; bool x[(1 << (n+1)) - 1];
    for (int i = 0; i < (1 << n); i++) { cin >> l[POS(i)]; x[POS(i)] = false; }
    for (int i = POS(0)-1; i >= 0; i--) x[i] = !x[LCHILD(i)];
    for (int i = POS(0)-1; i >= 0; i--) l[i] = x[i] ? (l[LCHILD(i)]|l[RCHILD(i)]) : (l[LCHILD(i)]^l[RCHILD(i)]);
    while (m--) {
        unsigned int pos, val; cin >> pos >> val; pos--; l[POS(pos)] = val;
        for (unsigned int c = PARENT(POS(pos)); c != 0; c = PARENT(c))
            l[c] = x[c] ? (l[LCHILD(c)]|l[RCHILD(c)]) : (l[LCHILD(c)]^l[RCHILD(c)]);
        l[0] = x[0] ? (l[LCHILD(0)]|l[RCHILD(0)]) : (l[LCHILD(0)]^l[RCHILD(0)]);
        cout << l[0] << endl;
    } return 0;
}