/**
 * @Problem
 * For any subarray (l,r) let x be the XOR of maximum two numbers.
 * Find the maximum of all such x's for any subarray in the given array.
 * @Reference
 * https://codeforces.com/contest/281/problem/D
 * two pointers, *2000
 */

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define SWAP(a, b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }

int main() {
    // Input Segment
    unsigned long long n, r = 0; cin >> n;
    vector<unsigned long long> list(n); for (int i = 0; i < n; i++) cin >> list[i];
    // Finding the result from Left to Right (If max comes after second max)
    stack<unsigned long long> st;
    for (auto item : list) {
        while (!st.empty() && st.top() < item) {
            r = MAX(r, item ^ st.top());
            st.pop();
        }
        st.push(item);
    }
    // Finding the result from Right to Left (If max comes before second max)
    st.empty();
    for (int i = 0; i < list.size() / 2; i++) SWAP(list[i], list[list.size()-1-i]);
    for (int i = 0; i < n; i++) {
        while (!st.empty() && st.top() < list[i]) {
            r = MAX(r, list[i] ^ st.top());
            st.pop();
        }
        st.push(list[i]);
    }
    // Output Result
    cout << r << endl;
}