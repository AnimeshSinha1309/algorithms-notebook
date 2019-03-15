#include <iostream>
#include <queue>
using namespace std;

int main() {
    int n; cin >> n; bool grid[n][n]; queue<pair<int, int>> q;
    for (int i = 0; i < n; i++) { string t; cin >> t; for (int j = 0; j < n; j++) grid[i][j] = t[j] == '.'; }
    pair<int,int> start, goal; cin >> start.first >> start.second >> goal.first >> goal.second;


}