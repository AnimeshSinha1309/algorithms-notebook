/**
 * @Problem
 * Given a string, load each character onto a stack and pop to a resultant string
 * to get the lexicographically minimal string.
 * @Reference
 * https://codeforces.com/contest/797/problem/C
 * data structures, greedy, strings, *1700
 */

#include <iostream>
#include <stack>
using namespace std;
#define MIN(a, b) (a < b ? a : b)

int main() {
    string input; cin >> input; stack<char> stack; unsigned long n = input.size();
    char min[n]; min[n-1] = input[n-1]; for (int i = (int)n-2; i >= 0; i--) min[i] = MIN(input[i], min[i+1]);
    for (int i = 0; i < n; i++) {
        while(!stack.empty() && stack.top() <= min[i]) { cout << stack.top(); stack.pop(); };
        stack.push(input[i]);
    } while(!stack.empty()) { cout << stack.top(); stack.pop(); }
}