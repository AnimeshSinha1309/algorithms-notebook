#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main() {
    unsigned int n, q; cin >> n >> q; vector<int> list(n); for (int i = 0; i < n; i++) cin >> list[i];
    while(q--) {
        int d, min = INT32_MAX; cin >> d; deque<int> max;
        for (int i = 0; i < n; i++) {
            while (!max.empty() && i - max.front() >= d) max.pop_front();
            while (!max.empty() && list[i] > list[max.back()]) max.pop_back(); max.push_back(i);
            if (i >= d-1) min = (min > list[max.front()]) ? list[max.front()] : min;
        } cout << min << endl;
    }
}