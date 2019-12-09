/**
 * @Problem
 * Given a list of teachers, each arrives on day Di, wants to take Ti classes,
 * and has sadness Si for each day he cannot take class. Minimize total sadness.
 * @Reference
 * https://www.codechef.com/JULY17/problems/IPCTRAIN
 * admin2, easy, greedy, heap, july17, stl
 */

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct lecturer {
    long long int d, t, s;
};

int main() {
    int tests;
    cin >> tests;
    while (tests--) {
        unsigned long long int n, d;
        cin >> n >> d;
        vector<lecturer> list(n);
        d++;
        auto compareD = [](lecturer a, lecturer b) { return a.d < b.d; };
        auto compareS = [](lecturer a, lecturer b) { return a.s < b.s; };
        priority_queue<lecturer, vector<lecturer>, decltype(compareS)> q(
            compareS);
        for (int i = 0; i < n; i++)
            cin >> list[i].d >> list[i].t >> list[i].s;
        sort(list.begin(), list.end(), compareD);
        for (int i = 0; i < n; i++) {
            q.push(list[i]);
            long long int reduction =
                ((i == n - 1) ? d : list[i + 1].d) - list[i].d;
            while (reduction > 0 && !q.empty()) {
                if (reduction < q.top().t) {
                    lecturer copy = q.top();
                    copy.t -= reduction;
                    q.pop();
                    q.push(copy);
                    reduction = 0;
                } else {
                    reduction -= q.top().t;
                    q.pop();
                }
            }
        }
        long long int sadness = 0;
        while (!q.empty()) {
            sadness += q.top().t * q.top().s;
            q.pop();
        }
        cout << sadness << endl;
    }
    return 0;
}