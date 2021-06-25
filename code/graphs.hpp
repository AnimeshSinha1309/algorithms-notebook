#include "template.hpp"

pair<vll, vll> dijkstra(vector<vpl> &adj, int from) {
    int n = adj.size();
    vll dist(n, INT64_MAX), parent(n, -1);
    priority_queue<pll, vpl, greater<>> q;
    dist[from] = 0, q.emplace(0, from);
    while (!q.empty()) {
        ll n = q.top().second, d = q.top().first;
        q.pop();
        if (d > dist[n])
            continue;
        for (auto edge : adj[n]) {
            if (d + edge.second < dist[edge.first]) {
                dist[edge.first] = d + edge.second;
                parent[edge.first] = n;
                q.emplace(d + edge.second, edge.first);
            }
        }
    }
    return {dist, parent};
}

vii topological_sort(mii &adj) {
    int n = adj.size();
    vii ans;
    vbl visited(n, false);
    ans.reserve(n);

    function<void(int)> dfs = [&](int node) {
        visited[node] = true;
        for (auto el : adj[node]) {
            if (!visited[el])
                dfs(el);
        }
        ans.push_back(node);
    };
    for (int i = 0; i < n; i++)
        if (!visited[i])
            dfs(i);
    std::reverse(ans.begin(), ans.end());
    return ans;
}

vii components(mii &adj) {
    int n = adj.size(), comp = 0;
    vii ans(n);
    vbl visited(n, false);

    function<void(int)> dfs = [&](int node) {
        visited[node] = true;
        for (auto el : adj[node]) {
            if (!visited[node])
                dfs(node);
        }
        ans[node] = comp;
    };
    for (int i = 0; i < n; i++) {
        if (!visited[i])
            dfs(i), comp++;
    }
    return ans;
}

pair<vll, vll> bellman_ford(vector<vpl> &adj, int from) {
    int n = adj.size();
    vll d(n, INT64_MAX), p(n, INT32_MAX);
    d[from] = 0;
    for (int i = 0; i < n; i++)
        for (int u = 0; u < n; u++)
            if (d[u] != INT64_MAX)
                for (const auto &e : adj[u]) {
                    if (d[u] + e.second < d[e.first]) {
                        if (i == n - 1) {
                            d[e.first] = INT64_MIN;
                        } else
                            d[e.first] = d[u] + e.second, p[e.first] = u;
                    }
                }
    return {d, p};
}

pair<vll, vll> spfa(vector<vpl> &adj, int from) {
    vll d(adj.size(), INT64_MAX), p(adj.size(), -1);
    vector<int> cnt_q(adj.size(), 0);
    vbl in_q(adj.size(), false);
    queue<int> q;
    d[from] = 0, q.push(from), in_q[from] = true, cnt_q[from]++;
    while (!q.empty()) {
        auto u = q.front();
        q.pop(), in_q[u] = false;
        for (auto e : adj[u]) {
            ll v = e.first, w = e.second;
            if (d[u] + w < d[v]) {
                d[v] = d[u] + w, p[v] = u;
                if (in_q[v])
                    continue;
                q.push(v), in_q[v] = true, cnt_q[v]++;
                if (cnt_q[v] >= adj.size()) {
                    d[v] = INT64_MIN;
                    return {d, p};
                }
            }
        }
    }
    return {d, p};
}

mll floyd_warshall(vector<vpl> &adj) {
    int n = adj.size();
    mll d(n, vll(n, INT64_MAX));
    for (int i = 0; i < n; i++) {
        d[i][i] = 0;
        for (auto v : adj[i])
            d[i][v.first] = v.second;
    }
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (d[i][k] != INT64_MAX && d[k][j] != INT64_MAX)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    return d;
}

pair<ll, vll> prims_mst(vector<vpl> &adj) {
    int n = adj.size();
    priority_queue<pll, vpl, greater<>> q;
    vll costs(n), parents(n);
    vbl visited(n, false);
    for (int i = 0; i < n; i++) {
        if (!visited[i])
            q.emplace(INT32_MAX, i);
        while (!q.empty()) {
            ll w = q.top().first, n = q.top().second;
            q.pop();
            visited[n] = true;
            for (const auto &edge : adj[n])
                if (!visited[edge.second]) {
                    costs[n] = w, parents[n] = n;
                    q.emplace(edge.second, edge.first);
                }
        }
    }
    ll sum = accumulate(costs.begin(), costs.end(), 0);
    return {sum, parents};
}