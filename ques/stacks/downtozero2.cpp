#include <cmath>
using namespace std;

int main() {
    int t; cin >> t; while (t--) {
        int result[1000100]; for (int i = 0; i < 1000100; i++) result[i] = -1; result[0] = 0; result[3] = 10;
        int n; cin >> n; queue<pair<int,int>> q; q.push(pair<int,int>(n, 0));
        while (!q.empty()) {
            if (result[q.front().first] == -1) { cout << result + q.front().second << endl; break; }
            q.push(pair<int,int>(q.front().first - 1, q.front().second + 1));
            for (int i = 2; i <= sqrt(q.front().first); i++) {
                if (q.front().first % i == 0)
                    q.push(pair<int,int>(q.front().first / i, q.front().second + 1));
            } q.pop();
        }
    }
}