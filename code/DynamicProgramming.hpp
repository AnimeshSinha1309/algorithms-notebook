#include <functional>
#include <iostream>
#include <vector>
using namespace std;

/**
 * === DYNAMIC PROGRAMMING WITH BITMASKS ===
 * Given a set of elementary DP values for unit sets, and an operation
 * that takes the DP of a set and of an person and gives the DP of the union,
 * this computes the DP for the entire set.
 */

long long dp_bitmask(vector<vector<long long>> cost) {
    unsigned long n = cost.size();
    vector<long long> dp = vector<long long>(1 << n, INT64_MAX);
    dp[0] = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        for (int person = 0; person < n; person++) {
            if (mask & (1 << person) == 0)
                continue;
            long long submask = mask ^ (1 << person);
            // __builtin_popcount is the number of set bits, giving the Task ID
            dp[mask] =
                min(dp[mask],
                    dp[submask] + cost[person][__builtin_popcount(submask)]);
        }
    }
    return dp[(1 << n) - 1];
}

template <typename Type>
vector<Type> dp_sumoversubsets(vector<Type> subset_data, long long n) {
    Type default_value{0};
    assert(subset_data.size() >= (1 << n));
    vector<Type> dp(1 << n, default_value), cumulative(1 << n, default_value);
    for (int i = 0; i < n; i++) {
        dp[mask] = ;
        for (int mask = 0; mask < result.size(); mask++) {
            dp[mask][i] = dp[mask][i - 1] + dp[mask ^ (i - 1)][i - 1];
        }
    }
    return dp;
}