#include "../ref/template.hpp"

struct HuffmanTree {
    vll prob;
    vector<pair<int, bool>> parent;
    vector<int> depth;
    ll size, total_prob;

    HuffmanTree(const vll &p) {
        // Initialize everything
        prob = vll(p);
        total_prob = accumulate(prob.begin(), prob.end(), 0);
        size = p.size();
        parent = vector<pair<int, bool>>(size * 2, {-1, false});
        depth = vector<int>(size);
        // Get the parents of each node by greedy combination
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q;
        for (int i = 0; i < size; i++)
            q.emplace(prob[i], i);
        int next_free_node = size;
        for (int i = 0; q.size() >= 2; i++) {
            pair<int, int> top1 = q.top();
            q.pop();
            pair<int, int> top2 = q.top();
            q.pop();
            parent[top1.second] = {next_free_node, false};
            parent[top2.second] = {next_free_node, true};
            q.emplace(top1.first + top2.first, next_free_node);
            next_free_node++;
        }
        parent.resize(next_free_node);
        // Get all the children
        vector<pair<int, int>> children(2 * size, {-1, -1});
        for (int i = 0; i < 2 * size; i++) {
            if (parent[i].first != -1) {
                if (children[parent[i].first].first == -1)
                    children[parent[i].first].first = i;
                else if (children[parent[i].first].second == -1)
                    children[parent[i].first].second = i;
            }
        }
        // Compute the depths using a dfs
        stack<int> dfs;
        dfs.push(q.top().second);
        depth[q.top().second] = 0;
        while (!dfs.empty()) {
            ll top = dfs.top();
            dfs.pop();
            if (children[top].first != -1) {
                depth[children[top].first] = depth[top] + 1;
                dfs.push(children[top].first);
            }
            if (children[top].second != -1) {
                depth[children[top].second] = depth[top] + 1;
                dfs.push(children[top].second);
            }
        }
    }

    string get_string(int letter) {
        string st;
        for (int cur = letter; letter != -1; letter = parent[letter].first)
            st += (parent[letter].first ? "0" : "1");
        return st;
    }

    static ll encoding_length(const vll &p) {
        priority_queue<ll, vector<ll>, greater<>> q;
        for (auto el : p)
            q.push(el);
        ll ans = 0;
        while (q.size() >= 2) {
            ll top1 = q.top();
            q.pop();
            ll top2 = q.top();
            q.pop();
            ans += top1 + top2;
            q.push(top1 + top2);
        }
        return ans;
    }
};
