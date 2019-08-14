#ifndef CODE_GRAPH_H
#define CODE_GRAPH_H

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <stack>
#include <queue>

using namespace std;

template <class Type>
class Graph
{
public:
    struct Node
    {
        template <class Return>
        Return dfs(function<Return(Node *, Return)> operation, unsigned long operationCode, Return defaultValue)
        {
            Return accumulator = defaultValue;
            if (this->previousOperation == operationCode)
                return accumulator;
            this->previousOperation = operationCode;
            for (auto node : this->adjacent)
            {
                accumulator = operation(this, node.first->dfs(operation, operationCode, defaultValue));
            }
            return accumulator;
        }
        void dfs(function<void(Node *)> operation, unsigned long operationCode)
        {
            if (this->previousOperation == operationCode)
                return;
            this->previousOperation = operationCode;
            operation(this);
            for (auto node : this->adjacent)
            {
                node.first->dfs(operation, operationCode);
            }
        }
        unsigned long previousOperation = 0;
        unsigned long id = 0;
        Type data;
        vector<pair<Node *, long long>> adjacent;
        Node *parent = nullptr;
    };
    enum CONFIGURATION
    {
        BIDIRECTIONAL,
        UNIDIRECTIONAL
    };

private:
    unsigned long operationCode = 0;
    CONFIGURATION CONFIG;

public:
    vector<Node> nodes;
    explicit Graph(const vector<Type> data = {}, CONFIGURATION configuration = BIDIRECTIONAL)
    {
        nodes = vector<Node>(data.size());
        for (int i = 0; i < data.size(); i++)
        {
            nodes[i].data = data[i];
            nodes[i].id = i + 1;
        }
        this->CONFIG = configuration;
    }

    void addNode(Type data)
    {
        Node temporary;
        temporary.data = data;
        nodes.push_back(temporary);
    }

    void addEdge(unsigned long from, unsigned long to, long long weight = 1)
    {
        nodes[from].adjacent.emplace_back(&nodes[to], weight);
        if (CONFIG == BIDIRECTIONAL)
            nodes[to].adjacent.emplace_back(&nodes[from], weight);
    }

    template <class Return>
    Return dfs(function<Return(Node *, Return)> operation, unsigned long at, Return defaultValue)
    {
        return nodes[at].template dfs<Return>(operation, ++operationCode, defaultValue);
    }

    void dfs(function<void(Node *)> operation, unsigned long at)
    {
        return nodes[at].dfs(operation, ++operationCode);
    }

    vector<vector<unsigned long>> components()
    {
        if (CONFIG == UNIDIRECTIONAL)
            throw "Strongly Connected components have not been implemented.";
        vector<vector<unsigned long>> result;
        operationCode++;
        for (unsigned long i = 0; i < nodes.size(); i++)
        {
            if (nodes[i].previousOperation == operationCode)
                continue;
            vector<unsigned long> component;
            stack<Node *> process;
            process.push(&nodes[i]);
            component.push_back(i);
            nodes[i].previousOperation = operationCode;
            while (!process.empty())
            {
                Node *processing = process.top();
                process.pop();
                for (pair<Node *, long long> child : processing->adjacent)
                {
                    if (child.first->previousOperation != operationCode)
                    {
                        process.push(child.first);
                        component.push_back(child.first->id - 1);
                        child.first->previousOperation = operationCode;
                    }
                }
            }
            result.push_back(component);
        }
        return result;
    }
    vector<unsigned long> topologicalSort()
    {
        vector<unsigned long> inDegree(nodes.size(), 0);
        vector<unsigned long> result;
        for (auto node : nodes)
            for (auto route : node.adjacent)
                inDegree[route.first->id - 1]++;
        queue<unsigned long> process;
        for (unsigned long i = 0; i < nodes.size(); i++)
        {
            if (inDegree[i] == 0)
            {
                process.push(i);
                result.push_back(i);
            }
        }
        while (!process.empty())
        {
            unsigned long processing = process.front();
            process.pop();
            for (auto route : nodes[processing].adjacent)
            {
                inDegree[route.first->id - 1]--;
                if (inDegree[route.first->id - 1] == 0)
                {
                    process.push(route.first->id - 1);
                    result.push_back(route.first->id - 1);
                }
            }
        }
        return result;
    }

    pair<vector<long long>, vector<unsigned long>> dijkstra(vector<unsigned long> from)
    {
        unsigned long n = nodes.size();
        vector<long long> distance(n, INT64_MAX);
        vector<unsigned long> parent(n, INT32_MAX);
        priority_queue<pair<Node *, long long>, vector<pair<Node *, long long>>, greater<>> dijkstra;
        for (auto index : from)
        {
            distance[index] = 0;
            dijkstra.push({&nodes[index], 0});
        }
        while (!dijkstra.empty())
        {
            pair<Node *, long long> current = dijkstra.top();
            dijkstra.pop();
            if (current.second > distance[current.first->id - 1])
                continue;
            for (auto edge : current.first->adjacent)
            {
                if (current.second + edge.second < distance[edge.first->id - 1])
                {
                    distance[edge.first->id - 1] = current.second + edge.second;
                    parent[edge.first->id - 1] = current.first->id - 1;
                    dijkstra.push({edge.first, current.second + edge.second});
                }
            }
        }
        return {distance, parent};
    }

    pair<vector<long long>, vector<unsigned long>> bellmanFord(vector<unsigned long> from)
    {
        unsigned long n = nodes.size();
        vector<long long> distances(n, INT64_MAX);
        vector<unsigned long> parent(n, INT32_MAX);
        // Bellman Ford Algorithm
        for (unsigned long &i : from)
            distances[i] = 0;
        for (int i = 0; i < n - 1; i++)
        {
            for (unsigned long source = 0; source < n - 1; source++)
            {
                if (distances[source] == INT64_MAX)
                    continue;
                for (const auto &edge : nodes[source].adjacent)
                {
                    unsigned long sink = edge.first->id - 1;
                    if (distances[source] + edge.second < distances[sink])
                    {
                        distances[sink] = distances[source] + edge.second;
                        parent[sink] = source;
                    }
                }
            }
        }
        // Checking for negative cycles and putting -1 if it exists.
        for (unsigned long source = 0; source < n - 1; source++)
        {
            for (const auto &edge : nodes[source].adjacent)
            {
                unsigned long sink = edge.first->id - 1;
                if (distances[source] + edge.second < distances[sink])
                {
                    for (unsigned long i : from)
                        distances[i] = -1;
                    return {distances, parent};
                }
            }
        }
        return {distances, parent};
    }

    vector<vector<long long>> floydWarshall()
    {
        unsigned long n = nodes.size();
        vector<vector<long long>> distances(n, vector<long long>(n, INT64_MAX));
        for (int i = 0; i < n; i++)
            distances[i][i] = 0;
        for (int i = 0; i < n; i++)
            for (auto route : nodes[i].adjacent)
                distances[i][route.first->id - 1] = route.second;
        for (int k = 0; k < n; k++)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (distances[i][k] == INT64_MAX || distances[k][j] == INT64_MAX)
                        continue;
                    distances[i][j] = min(distances[i][j], distances[i][k] + distances[k][j]);
                }
            }
        }
        return distances;
    }

    pair<long long, vector<long long>> primsMST()
    {
        if (CONFIG == UNIDIRECTIONAL)
            throw "Cannot find unique MST on a unidirectional graph";
        priority_queue<pair<Node *, long long>, vector<pair<Node *, long long>>, greater<>> routes;
        vector<long long> costs(nodes.size());
        operationCode++;
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i].previousOperation != operationCode)
                routes.push({&nodes[i], INT64_MAX});
            while (!routes.empty())
            {
                pair<Node *, long long> best = routes.top();
                routes.pop();
                if (best.first->previousOperation != operationCode)
                    costs[best.first->id - 1] = best.second;
                best.first->previousOperation = operationCode;
                for (const auto &path : best.first->adjacent)
                    if (path.first->previousOperation != operationCode)
                        routes.push(path);
            }
        }
        costs[0] = 0;
        long long sum = 0;
        for (const long long &cost : costs)
        {
            if (cost >= INT64_MAX)
            {
                sum = cost;
                break;
            }
            else
                sum += cost;
        }
        return {sum, costs};
    }
};

#endif //CODE_GRAPH_H
