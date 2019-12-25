#include "template.hpp"

#define AVG_CASE
#ifdef AVG_CASE
struct Edge {
    ll from, to, cap, flow, index;
    Edge(ll dfrom, ll dto, ll dcap, ll dflow, ll dindex)
        : from(dfrom), to(dto), cap(dcap), flow(dflow), index(dindex) {
    }
};
struct PushRelabel {
    ll N;
    vector<vector<Edge>> G;
    vector<ll> excess;
    vector<ll> dist, active, count;
    queue<ll> Q;
    PushRelabel(ll dN)
        : N(dN), G(N), excess(N), dist(N), active(N), count(2 * N) {
    }
    void addEdge(ll from, ll to, ll cap) {
        G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
        if (from == to)
            G[from].back().index++;
        G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
    }
    void Enqueue(ll v) {
        if (!active[v] && excess[v] > 0) {
            active[v] = true;
            Q.push(v);
        }
    }
    void Push(Edge &e) {
        ll amt = ll(min(excess[e.from], ll(e.cap - e.flow)));
        if (dist[e.from] <= dist[e.to] || amt == 0)
            return;
        e.flow += amt;
        G[e.to][e.index].flow -= amt;
        excess[e.to] += amt;
        excess[e.from] -= amt;
        Enqueue(e.to);
    }
    void Gap(ll k) {
        for (ll v = 0; v < N; v++) {
            if (dist[v] < k)
                continue;
            count[dist[v]]--;
            dist[v] = max(dist[v], N + 1);
            count[dist[v]]++;
            Enqueue(v);
        }
    }
    void Relabel(ll v) {
        count[dist[v]]--;
        dist[v] = 2 * N;
        for (ll i = 0; i < (ll)G[v].size(); i++)
            if (G[v][i].cap - G[v][i].flow > 0)
                dist[v] = min(dist[v], dist[G[v][i].to] + 1);
        count[dist[v]]++;
        Enqueue(v);
    }
    void Discharge(ll v) {
        for (ll i = 0; excess[v] > 0 && i < (ll)G[v].size(); i++)
            Push(G[v][i]);
        if (excess[v] > 0) {
            if (count[dist[v]] == 1)
                Gap(dist[v]);
            else
                Relabel(v);
        }
    }
    ll maxFlow(ll s, ll t) {
        count[0] = N - 1;
        count[N] = 1;
        dist[s] = N;
        active[s] = active[t] = true;
        for (ll i = 0; i < (ll)G[s].size(); i++) {
            excess[s] += G[s][i].cap;
            Push(G[s][i]);
        }
        while (!Q.empty()) {
            ll v = Q.front();
            Q.pop();
            active[v] = false;
            Discharge(v);
        }
        ll totflow = 0;
        for (ll i = 0; i < (ll)G[s].size(); i++)
            totflow += G[s][i].flow;
        return totflow;
    }
};
#else
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
#endif
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