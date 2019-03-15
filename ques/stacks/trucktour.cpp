#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    unsigned int n; long long fuel = 0, index = 0; cin >> n;
    vector<int> amount(n), distance(n); for (int i = 0; i < n; i++) cin >> amount[i] >> distance[i];
    for (int i = 0; i < n; i++) {
        fuel += amount[i] - distance[i];
        if (fuel < 0) { fuel = 0; index = i + 1; }
    } cout << index << endl;
}