/**
 * @Problem
 * For two wires entangled with each other, given which one is on top after every loop,
 * Figure out whether they can be disentangled.
 * @Reference
 * https://codeforces.com/contest/343/problem/B
 * data structures, greedy, implementation, *1600
 */

#include <iostream>
#include <stack>
using namespace std;

int main()
{
    stack<int> st; string input; cin >> input;
    for (char i : input) { if (st.empty() || st.top() != i) st.push(i); else st.pop(); }
    cout << (st.empty() ? "Yes" : "No") << endl;
}