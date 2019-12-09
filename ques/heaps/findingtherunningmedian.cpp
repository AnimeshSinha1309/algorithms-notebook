/**
 * @Problem
 * As elements keep being added to a set, keep finding it's median.
 * @Reference
 * https://www.hackerrank.com/challenges/find-the-running-median/problem
 * data structures, heaps, hard
 */

#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main() {
    unsigned int n;
    cin >> n;
    vector<int> list(n);
    for (int i = 0; i < n; i++)
        cin >> list[i];
    int top;
    cout << setprecision(1) << fixed;
    // Maintain a min-heap of elements greater than median, and max-heap of
    // elements less then the median.
    priority_queue<int, vector<int>, std::less<>> less;
    priority_queue<int, vector<int>, std::greater<>> more;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            if (!less.empty() && list[i] < less.top()) {
                top = less.top();
                less.pop();
                less.push(list[i]);
            } else if (!more.empty() && list[i] > more.top()) {
                top = more.top();
                more.pop();
                more.push(list[i]);
            } else {
                top = list[i];
            }
            cout << (double)top << endl;
        } else {
            if (top > list[i]) {
                more.push(top);
                less.push(list[i]);
            } else {
                more.push(list[i]);
                less.push(top);
            }
            cout << ((double)less.top() + (double)more.top()) / 2 << endl;
        }
    }
}