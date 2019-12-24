#include <template.hpp>

class Dinics {
   public:
    typedef int FT;           // can use float/doublestatic
    const FT INF = 1e9;       // maximum capacity
    static const FT EPS = 0;  // minimum capacity/flow change
    int nodes, src, dest;
    vector<int> dist, q, work;
    struct Edge {
        int to, rev;
        FT f, cap;
    };
    vector<vector<Edge> > g;
    bool dinic_bfs() {
        fill(dist.begin(), dist.end(), -1);
        dist[src] = 0;
        int qt = 0;
        q[qt++] = src;
        for (int qh = 0; qh < qt; qh++) {
            int u = q[qh];
            for (int j = 0; j < (int)g[u].size(); j++) {
                Edge &e = g[u][j];
                int v = e.to;
                if (dist[v] < 0 && e.f < e.cap)
                    dist[v] = dist[u] + 1;
                q[qt++] = v;
            }
        }
        return dist[dest] >= 0;
    }
    int dinic_dfs(int u, int f) {
        if (u == dest)
            return f;
        for (int &i = work[u]; i < (int)g[u].size(); i++) {
            Edge &e = g[u][i];
            if (e.cap <= e.f)
                continue;
            int v = e.to;
            if (dist[v] == dist[u] + 1) {
                FT df = dinic_dfs(v, min(f, e.cap - e.f));
                if (df > 0) {
                    e.f += df, g[v][e.rev].f -= df;
                    return df;
                }
            }
        }
        return 0;
    }
    Dinics(int n) : dist(n, 0), q(n, 0), work(n, 0), g(n), nodes(n) {
    }  // *** s->t (cap); t->s (rcap)
    void addEdge(int s, int t, FT cap, FT rcap = 0) {
        g[s].push_back({t, (int)g[t].size(), 0, cap});
        g[t].push_back({s, (int)g[s].size() - 1, 0, rcap});
    }  // ***
    FT maxFlow(int _src, int _dest) {
        src = _src, dest = _dest;
        FT result = 0, delta;
        while (dinic_bfs()) {
            fill(work.begin(), work.end(), 0);
            while ((delta = dinic_dfs(src, INF)) > EPS)
                result += delta;
        }
        return result;
    }
};

class HopcroftKarp {
   public:
    static const int INF = 1e9;
    int U, V, nil;
    vector<int> pairU, pairV, dist;
    vector<vector<int> > adj;
    bool bfs() {
        queue<int> q;
        for (int u = 0; u < U; u++)
            if (pairU[u] == nil)
                dist[u] = 0, q.push(u);
            else
                dist[u] = INF;
        dist[nil] = INF;
        while (not q.empty()) {
            int u = q.front();
            q.pop();
            if (dist[u] >= dist[nil])
                continue;
            for (int v : adj[u])
                if (dist[pairV[v]] == INF)
                    dist[pairV[v]] = dist[u] + 1, q.push(pairV[v]);
        }
        return dist[nil] != INF;
    }
    bool dfs(int u) {
        if (u == nil)
            return true;
        for (int v : adj[u])
            if (dist[pairV[v]] == dist[u] + 1)
                if (dfs(pairV[v])) {
                    pairV[v] = u, pairU[u] = v;
                    return true;
                }
        dist[u] = INF;
        return false;
    }

   public:
    HopcroftKarp(int U_, int V_) {
        nil = U = V = max(U_, V_);
        adj.resize(U + 1);
        dist.resize(U + 1);
        pairU.resize(U + 1);
        pairV.resize(V);
    }
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }
    int maxMatch() {
        fill(pairU.begin(), pairU.end(), nil);
        fill(pairV.begin(), pairV.end(), nil);
        int res = 0;
        while (bfs())
            for (int u = 0; u < U; u++)
                if (pairU[u] == nil && dfs(u))
                    res++;
        return res;
    }
};