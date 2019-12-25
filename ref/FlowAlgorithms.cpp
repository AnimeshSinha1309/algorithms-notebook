#include "template.hpp"

struct Edge {
    int u, v;
    ll cap, flow;
    Edge() : u(0), v(0), cap(0), flow(0) {
    }
    Edge(int uu, int vv, ll ccap) : u(uu), v(vv), cap(ccap), flow(0) {
    }
};
struct Dinic {
    int N;
    vector<Edge> E;
    vector<vector<int>> g;
    vector<int> d, pt;
    Dinic(int NN) : N(NN), E(0), g(N), d(N), pt(N) {
    }
    void addEdge(int u, int v, ll cap, ll rcap = 0) {
        if (u != v) {
            E.emplace_back(Edge(u, v, cap));
            g[u].emplace_back(E.size() - 1);
            E.emplace_back(Edge(v, u, rcap));
            g[v].emplace_back(E.size() - 1);
        }
    }
    bool BFS(int S, int T) {
        queue<int> q({S});
        fill(d.begin(), d.end(), N + 1);
        d[S] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (u == T)
                break;
            for (int k : g[u]) {
                Edge &e = E[k];
                if (e.flow < e.cap && d[e.v] > d[e.u] + 1) {
                    d[e.v] = d[e.u] + 1;
                    q.emplace(e.v);
                }
            }
        }
        return d[T] != N + 1;
    }

    ll DFS(int u, int T, ll flow = -1) {
        if (u == T || flow == 0)
            return flow;
        for (int &i = pt[u]; i < (int)g[u].size(); ++i) {
            Edge &e = E[g[u][i]];
            Edge &oe = E[g[u][i] ^ 1];
            if (d[e.v] == d[e.u] + 1) {
                ll amt = e.cap - e.flow;
                if (flow != -1 && amt > flow)
                    amt = flow;
                if (ll pushed = DFS(e.v, T, amt)) {
                    e.flow += pushed;
                    oe.flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }
    ll maxFlow(int S, int T) {
        ll total = 0;
        while (BFS(S, T)) {
            fill(pt.begin(), pt.end(), 0);
            while (ll flow = DFS(S, T))
                total += flow;
        }
        return total;
    }
};

class HopcroftKarp {
   public:
    static const int INF = 1e9;
    int U, V, nil;
    vector<int> pairU, pairV, dist;
    vector<vector<int>> adj;
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