#include <iostream>
#include <vector>
#include <queue>
#define MAX(a, b) (a > b ? a : b)
using namespace std;

struct point { int x = 0, y = 0; int val = 0; point(int _x, int _y, int _val) { x = _x; y = _y; val = _val; } };

int main() {
    int q, ans = 0; cin >> q; vector<point> points;
    auto compare = [](point a, point b) { return a.val < b.val; };
    auto equals = [](point a, point b) { return a.x == b.x && a.y == b.y; };
    auto distance = [](point a, point b) { return abs(a.x - b.x) + abs(a.y - b.y); };
    priority_queue<point, vector<point>, decltype(compare)> ptPP(compare), delPP(compare), ptPM(compare),
            delPM(compare), ptMM(compare), delMM(compare), ptMP(compare), delMP(compare);
    while (q--) {
        char ch; point p(0, 0, 0); cin >> ch;
        switch (ch) {
            case '+':
                cin >> p.x >> p.y; p.x ^= ans; p.y ^= ans;
                ptPP.push(point(p.x, p.y,   p.x + p.y));
                ptPM.push(point(p.x, p.y,   p.x - p.y));
                ptMM.push(point(p.x, p.y, - p.x - p.y));
                ptMP.push(point(p.x, p.y, - p.x + p.y));
                points.push_back(p);
                break;
            case '-':
                int n; cin >> n; n ^= ans; p = points[n-1];
                delPP.push(point(p.x, p.y,   p.x + p.y));
                delPM.push(point(p.x, p.y,   p.x - p.y));
                delMM.push(point(p.x, p.y, - p.x - p.y));
                delMP.push(point(p.x, p.y, - p.x + p.y));
                while (!ptPP.empty() && equals(ptPP.top(), delPP.top())) { ptPP.pop(); delPP.pop(); }
                while (!ptPM.empty() && equals(ptPM.top(), delPM.top())) { ptPM.pop(); delPM.pop(); }
                while (!ptMM.empty() && equals(ptMM.top(), delMM.top())) { ptMM.pop(); delMM.pop(); }
                while (!ptMP.empty() && equals(ptMP.top(), delMP.top())) { ptMP.pop(); delMP.pop(); }
                break;
            case '?':
                cin >> p.x >> p.y; p.x ^= ans; p.y ^= ans;
                ans = MAX(
                        MAX(distance(p, ptPP.top()), distance(p, ptPM.top())),
                        MAX(distance(p, ptMM.top()), distance(p, ptMP.top())) );
                cout << ans << endl;
                break;
            default:
                return 1;
        }
    }
}