/**
 * @Problem
 * Find the Kth maximum sum of a subarray in a given array.
 * @Reference
 * https://www.codechef.com/problems/KSUBSUM
 * cook18, easy, flying_ant
 */

#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

vector<long long> merge(vector<long long> a, vector<long long> b,
                        unsigned long k) {
    vector<long long> result(k);
    for (int i = 0, x = 0, y = 0; i < k; i++) {
        (x < a.size() && (y >= b.size() || a[x] > b[y])) ? (result[i] = a[x++])
                                                         : (result[i] = b[y++]);
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int tests;
    cin >> tests;
    while (tests--) {
        unsigned int n, k1, k2, k3;
        cin >> n >> k1 >> k2 >> k3;
        long long input[n];
        for (int i = 0; i < n; i++)
            cin >> input[i];
        long long ps[n];
        ps[0] = input[0];
        for (int i = 1; i < n; i++)
            ps[i] = ps[i - 1] + input[i];
        vector<long long> maxSum(k3, INT32_MIN), minPre({0});
        for (int i = 0; i < n; i++) {
            // Get the list of max-sums using older max-sums and current prefix
            // - lowest k prefixes
            vector<long long> sums;
            sums.reserve(minPre.size());
            for (auto el : minPre)
                sums.push_back(ps[i] - el);
            maxSum = merge(maxSum, sums, k3);
            // Maintain a k size sorted array of the lowest prefixes, in
            // ascending order
            minPre.push_back(ps[i]);
            for (unsigned long d = minPre.size() - 1;
                 d > 0 && minPre[d - 1] > minPre[d]; d--) {
                minPre[d] ^= minPre[d - 1];
                minPre[d - 1] ^= minPre[d];
                minPre[d] ^= minPre[d - 1];
            }
            while (minPre.size() > k3)
                minPre.pop_back();
        }
        cout << maxSum[k1 - 1] << " " << maxSum[k2 - 1] << " " << maxSum[k3 - 1]
             << endl;
    }
    return 0;
}