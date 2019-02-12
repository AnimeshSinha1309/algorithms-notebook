/**
 * @Problem
 * In a Sequence of Open and Closed Brackets, Identify the longest well-bracketed subsequence
 * and count the total number of Disjoint well-bracketed subsequences.
 * @Reference
 * https://codeforces.com/contest/5/problem/C
 * constructive algorithms, data structures, dp, greedy, sorting, strings, *1900
 */

#include <iostream>
#include <stack>
using namespace std;

int main()
{
    string input; cin >> input; input = '-' + input; stack<int> st; st.push(0);
    for (int i = 1; i < input.length(); i++) {
        if (input[i] == '(') { st.push(i); }
        else if (input[i] == ')') { (input[st.top()] == '(') ? st.pop() : st.push(i); }
    } st.push((int) input.length());
    int count = 1, length = 0;
    while (st.size() > 1) {
        int l = st.top(); st.pop(); l -= st.top() + 1;
        if (length < l) { length = l; count = 1; } else if (length == l && l > 0) { count++; }
    } cout << length << " " << count << endl;
}

/**
 O 18 ) 2 ) 2 ( ( 16 ( 28 (( 8 ( 18
 */
