/**
 * @Problem
 * For any (l,r), let S = (((a & b) ^ (a | b)) & (a ^ b)), where a is minimum and
 * b is second minimum. Print the maximum possible value of S for any (l,r)
 * @Reference
 * https://www.hackerrank.com/challenges/and-xor-or/problem
 * stacks, medium, histogram
 */

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define SWAP(a, b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }
long long int EXPR(long long int a, long long int b) { return (((a & b) ^ (a | b)) & (a ^ b)); }

int main() {
    // Input Segment
    long long n, r = INT32_MIN; cin >> n;
    vector<long long> list((unsigned) n); for (int i = 0; i < n; i++) cin >> list[i];
    // Finding the result from Left to Right (If max comes after second max)
    stack<long long> st;
    for (auto item : list) {
        while (!st.empty() && st.top() > item) {
            r = MAX(r, EXPR(item, st.top()));
            st.pop();
        }
        st.push(item);
    }
    // Finding the result from Right to Left (If max comes before second max)
    st.empty();
    for (int i = 0; i < list.size() / 2; i++) SWAP(list[i], list[list.size()-1-i]);
    for (auto item : list) {
        while (!st.empty() && st.top() > item) {
            r = MAX(r, EXPR(item, st.top()));
            st.pop();
        }
        st.push(item);
    }
    // Output Result
    cout << r << endl;
}