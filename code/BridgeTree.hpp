#ifndef CODE_FLOW_ALGORITHMS_H
#define CODE_FLOW_ALGORITHMS_H

#include "../ref/template.hpp"

class BridgeTree {
    map<pair<int,int>, int> bridge;
    vector<int> depth;
    vector<vector<int>> gr, Tree;
    vector<bool> vst;
    int n, Nodes;

   public:
    BridgeTree(const vector<vector<int>> &Gr) {
        gr = Gr;
        n = gr.size();
        depth.assign(n, 0);
        vst.assign(n, false);
        Nodes = 1;
    }
    int dfs(int u, int pa, vector<bool> &vst, int d) {
        vst[u] = true;
        depth[u] = d;
        int mx = d;
        for (auto it : gr[u]) {
            if (!vst[it]) {
                int mx1 = dfs(it, u, vst, d + 1);
                mx = min(mx, mx1);
                if (mx1 > d) {
                    bridge[minmax(u, it)] = 1;
                }
            } else if (it != pa) {
                mx = min(mx, depth[it]);
            }
        }
        return mx;
    }
    void dfs2(int u, vector<bool> &vst, int node) {
        vst[u] = true;
        for (auto it : gr[u]) {
            if (!vst[it]) {
                int v1 = node;
                if (bridge.count(minmax(u, it))) {
                    v1 = Nodes++;
                    Tree[node].pb(v1);
                    Tree[v1].pb(node);
                }
                dfs2(it, vst, v1);
            }
        }
    }
    vector<vector<int>> tree() {
        dfs(0, -1, vst, 0);
        fill(all(vst), false);
        Tree.resize(sz(bridge) + 1);
        dfs2(0, vst, 0);
        return Tree;
    }
};


#endif  // CODE_FLOW_ALGORITHMS_H