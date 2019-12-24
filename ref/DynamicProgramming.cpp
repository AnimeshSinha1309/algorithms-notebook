#include <template.hpp>

class LineContainer {
   private:
    struct Line {
        mutable long long slope, constt, p;
        bool operator<(const Line &o) const {
            return slope < o.slope;
        }
        bool operator<(long long x) const {
            return p < x;
        }
    };
    multiset<Line, less<>> lines;
    // (for doubles, use inf = 1/.0, div(a,b) = a/b)
    bool __is_max_query = false;
    const long long inf = LLONG_MAX;
    long long __div(long long a, long long b) {  // floored division
        return a / b - ((a ^ b) < 0 && a % b);
    }
    bool __intersect(multiset<Line>::iterator x, multiset<Line>::iterator y) {
        if (y == lines.end()) {
            x->p = inf;
            return false;
        }
        if (x->slope == y->slope)
            x->p = x->constt > y->constt ? inf : -inf;
        else
            x->p = __div(y->constt - x->constt, x->slope - y->slope);
        return x->p >= y->p;
    }

   public:
    LineContainer(bool is_max = false) {
        this->__is_max_query = is_max;
    }
    void add(long long slope, long long constt) {
        if (!__is_max_query) {
            slope = -slope;
            constt = -constt;
        }
        auto z = lines.insert({slope, constt, 0}), y = z++, x = y;
        while (__intersect(y, z))
            z = lines.erase(z);
        if (x != lines.begin() && __intersect(--x, y))
            __intersect(x, y = lines.erase(y));
        while ((y = x) != lines.begin() && (--x)->p >= y->p)
            __intersect(x, lines.erase(y));
    }
    long long query(long long x) {
        assert(!lines.empty());
        auto l = *lines.lower_bound(x);
        return (l.slope * x + l.constt) * (__is_max_query ? 1 : -1);
    }
};

void dp_sos(vll &arr) {
    const int bitsize = 20;
    for (int i = 0; i < bitsize; ++i)
        for (int mask = 0; mask < (1 << bitsize); ++mask)
            if (mask & (1 << i))
                arr[mask] += arr[mask ^ (1 << i)];
}