/**
 * @Problem
 * Given an array, after each iteration all elements with smaller values to immediate left
 * get deleted. Find the number of iterations before there are no more deletions.
 * @Reference
 * https://www.hackerrank.com/challenges/poisonous-plants/problem
 * stacks, hard
 */

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#define MAX(a, b) (a > b ? a : b)
using namespace std;
struct X { long long val, shot; explicit X(long long n = 0, long long s = 0) { this->val = n; this->shot = s; } };

int main() {
    unsigned int n; cin >> n;
    vector<long long> list(n); for (int i = 0; i < n; i++) cin >> list[i]; reverse(list.begin(), list.end());
    stack<X> st; long count = 0, max = 0;
    for (auto el : list) {
        for(count = 0; !st.empty() && st.top().val > el; ) { count += MAX(st.top().shot-count, 1); st.pop(); }
        st.push(X(el, count)); max = MAX(max, count);
    } cout << max << endl;
}