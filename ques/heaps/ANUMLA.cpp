/**
 * @Problem
 * Sum of all subsets of a set is given, find the original set.
 * @Reference
 * https://en.cppreference.com/w/cpp/container/priority_queue
 * cook51, easy, greedy, heap, multiset
 */

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int tests; cin >> tests; while (tests--) {
        priority_queue<int, vector<int>, greater<>> all, sum;
        vector<int> result, acc; acc.push_back(0); sum.push(0);
        int n; cin >> n; for (int i = 0; i < (1<<n); i++) { int temp; cin >> temp; all.push(temp); }
        while (!all.empty()) {
            if (sum.empty() || all.top() != sum.top()) {
                int val = all.top(), size = (int) acc.size(); result.push_back(val);
                for (int i = 1; i < size; i++) sum.push(val + acc[i]);
                for (int i = 0; i < size; i++) acc.push_back(val + acc[i]);
                all.pop();
            } else {
                all.pop(); sum.pop();
            }
        } for (auto el : result) cout << el << " "; cout << endl;
    }
}