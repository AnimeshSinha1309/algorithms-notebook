#include <iostream>
#include <queue>
using namespace std;

int main() {
    queue<int> queue;
    int q; cin >> q; while (q--) {
        int t; cin >> t; switch(t) {
            case 1: int x; cin >> x; queue.push(x); break;
            case 2: queue.pop(); break;
            case 3: cout << queue.front() << endl; break;
            default: return 1;
        }
    }
}