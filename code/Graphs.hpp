#ifndef CODE_GRAPH_H
#define CODE_GRAPH_H

#include "../ref/template.hpp"

class Graph {
   public:
    enum NodeColor { VISITED, VISITING, UNVISITED };
    struct Node {
        int index;
        vpl adjacent;
        NodeColor color = UNVISITED;
    };
    vector<Node> list;
    int n;
    Graph(int n) {
        list.resize(n);
        for (int i = 0; i < n; i++)
            list[i].index = i;
        this->n = n;
    }
    void add_edge(int u, int v, long long w = 1, bool bidirectional = true) {
        list[u].adjacent.emplace_back(v, w);
        if (bidirectional)
            list[v].adjacent.emplace_back(u, w);
    }

    pair<vll, vll> dijkstra(vll from) {
        vll dist(n, INT64_MAX), parent(n, INT32_MAX);
        priority_queue<pll, vpl, greater<>> q;
        // q: (distance, node id); edge: (to, weight)
        for (auto index : from) {
            dist[index] = 0;
            q.emplace(0, index);
        }
        while (!q.empty()) {
            pll top = q.top();
            q.pop();
            if (top.first > dist[top.second])
                continue;
            for (auto edge : list[top.second].adjacent) {
                if (top.first + edge.second < dist[edge.first]) {
                    dist[edge.first] = top.first + edge.second;
                    parent[edge.first] = top.second - 1;
                    q.emplace(top.first + edge.second, edge.first);
                }
            }
        }
        return {dist, parent};
    }

    // Returns sorted vector of indices
    vector<int> topological_sort() {
        vector<int> in_degree(list.size(), 0), result;
        result.reserve(list.size());
        for (auto node : list)
            for (auto route : node.adjacent)
                in_degree[route.first - 1]++;
        queue<int> process;
        for (int i = 0; i < list.size(); i++) {
            if (in_degree[i] == 0) {
                process.push(i);
                result.push_back(i);
            }
        }
        while (!process.empty()) {
            int processing = process.front();
            process.pop();
            for (auto route : list[processing].adjacent) {
                in_degree[route.first - 1]--;
                if (in_degree[route.first - 1] == 0) {
                    process.push(route.first - 1);
                    result.push_back(route.first - 1);
                }
            }
        }
        return result;
    }

    mll components() {
        vbl visited(n);
        mll result(0);
        for (int i = 0; i < n; i++) {
            if (visited[i])
                continue;
            vll component;
            stack<ll> process;
            process.push(list[i].index);
            component.push_back(i);
            visited[i] = true;
            while (!process.empty()) {
                ll processing = process.top();
                process.pop();
                for (pll neighbor : list[processing].adjacent) {
                    if (!visited[neighbor.first]) {
                        process.push(neighbor.first);
                        component.push_back(neighbor.first);
                    }
                }
            }
            result.push_back(component);
        }
        return result;
    }

    pair<vll, vll> bellman_ford(vll from) {
        vll distances(n, INT64_MAX);
        vll parent(n, INT32_MAX);
        for (ll &i : from)
            distances[i] = 0;
        // relax all |E| edges, |V| - 1 times
        for (int i = 0; i < n - 1; i++) {
            for (int source = 0; source <= n - 1; source++) {
                if (distances[source] == INT64_MAX)
                    continue;
                for (const auto &edge : list[source].adjacent) {
                    ll sink = edge.first;
                    if (distances[source] + edge.second < distances[sink]) {
                        distances[sink] = distances[source] + edge.second;
                        parent[sink] = source;
                    }
                }
            }
        }
        // Checking for negative cycles and putting -1 if it exists.
        for (ll source = 0; source <= n - 1; source++) {
            for (const auto &edge : list[source].adjacent) {
                ll sink = edge.first;
                if (distances[source] < INT64_MAX &&
                    distances[source] + edge.second < distances[sink]) {
                    for (ll i : from)
                        distances[i] = INT64_MIN;
                    return {distances, parent};
                }
            }
        }
        return {distances, parent};
    }

    mll floyd_warshall() {
        mll distances(n, vll(n, INT64_MAX));
        for (int i = 0; i < n; i++)
            distances[i][i] = 0;
        for (int i = 0; i < n; i++)
            for (auto route : list[i].adjacent)
                distances[i][route.first] = route.second;
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (distances[i][k] == INT64_MAX ||
                        distances[k][j] == INT64_MAX)
                        continue;
                    distances[i][j] =
                        min(distances[i][j], distances[i][k] + distances[k][j]);
                }
            }
        }
        return distances;
    }

    pair<ll, vll> prims_mst() {
        priority_queue<pll, vpl, greater<>> routes;
        vll costs(n);
        vbl visited(n, false);
        for (int i = 0; i < n; i++) {
            if (!visited[i])
                routes.emplace(INT32_MAX, i);
            while (!routes.empty()) {
                pll best = routes.top();
                routes.pop();
                if (!visited[best.second])
                    costs[best.second] = best.first;
                visited[best.second] = false;
                for (const auto &path : list[best.second].adjacent)
                    if (!visited[path.second])
                        routes.push(path);
            }
        }
        ll sum = accumulate(costs.begin(), costs.end(), 0);
        return {sum, costs};
    }
};

#endif  // CODE_GRAPH_H
