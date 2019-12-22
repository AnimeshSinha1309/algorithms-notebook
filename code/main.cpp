#include <iostream>
#include "BinaryHeap.hpp"
#include "SegmentTree.hpp"
#include "Graphs.hpp"
#include "SearchString.hpp"
// #include "GeneralAlgortihms.h"
#include "FastFourier.hpp"
using namespace std;

int main() {
    // Polynomial x({1});
    // Polynomial y({1, 1});
    // Polynomial z = x * y;

    // for (ll i = 0; i < z.order; i++) {
    //     cout << z.coeff[i] << " ";
    // }
    // return 0;

    /**
     * Binary Trees
     */

    BinaryHeap<int> heap({7, 5, 3, 9, 1, 6}, greater<>());
    cout << "Binary Heap Sort: ";
    for (auto el : heap.sort())
        cout << el << " ";
    cout << endl;

    /**
     * Segment Tree
     */

    SegmentTree<int> segtree({2, 3, 8, 5, 6, 9},
                             [](int x, int y) { return x + y; }, 0);
    cout << "Segment Tree Sum: " << segtree.query(2, 4) << endl;

    /**
     * Graph Algorithms
     */

    Graph<int> graph1({10, 20, 30, 40, 50, 60, 70, 80, 90, 100},
                      Graph<int>::BIDIRECTIONAL);
    graph1.addEdge(0, 1);
    graph1.addEdge(0, 2);
    graph1.addEdge(0, 3);
    graph1.addEdge(0, 4);
    graph1.addEdge(0, 5);
    graph1.addEdge(0, 6);
    graph1.addEdge(0, 7);
    graph1.addEdge(0, 8);
    graph1.addEdge(7, 9);
    cout << "Graph Trace: ";
    graph1.dfs<void *>(
        [](Graph<int>::Node *node, void *sum) {
            cout << node->data << " ";
            return nullptr;
        },
        0, nullptr);
    cout << endl;

    Graph<int> graph2({10, 20, 30, 40, 50, 60, 70, 80},
                      Graph<int>::BIDIRECTIONAL);
    graph2.addEdge(0, 1);
    graph2.addEdge(1, 2);
    graph2.addEdge(2, 0);
    graph2.addEdge(3, 4);
    graph2.addEdge(4, 5);
    graph2.addEdge(4, 6);
    cout << "Connected Components: ";
    for (const auto &component : graph2.components()) {
        for (auto node : component)
            cout << node << " ";
        cout << "| ";
    }
    cout << endl;

    Graph<int> graph3({10, 20, 30, 40, 50}, Graph<int>::UNIDIRECTIONAL);
    cout << "Topological Sort: ";
    graph3.addEdge(0, 2);
    graph3.addEdge(0, 3);
    graph3.addEdge(2, 1);
    graph3.addEdge(3, 4);
    graph3.addEdge(1, 4);
    for (const auto &node : graph3.topologicalSort())
        cout << node << " ";
    cout << endl;

    const auto dijkstra = graph3.dijkstra({0});
    cout << "Dijkstra: ";
    for (auto i : dijkstra.first)
        cout << i << " ";
    cout << "| ";
    for (auto j : dijkstra.second)
        cout << j << " ";
    cout << endl;

    const auto bellman = graph3.bellmanFord({0});
    cout << "Bellman Ford: ";
    for (auto i : bellman.first)
        cout << i << " ";
    cout << "| ";
    for (auto j : bellman.second)
        cout << j << " ";
    cout << endl;

    const auto floyd = graph3.floydWarshall();
    cout << "Floyd Warshall: ";
    for (const auto &i : floyd) {
        for (const auto &j : i)
            cout << j << " ";
        cout << "| ";
    }
    cout << endl;

    const auto prim = graph2.primsMST();
    cout << "Prim's: ";
    cout << "[" << prim.first << "] ";
    for (auto j : prim.second)
        cout << j << " ";
    cout << endl;

    /**
     * String Algorithms
     */

    string st = "ABCDABD";
    SearchString str(st);
    const vector<long> result = str.search("ABC ABCDAB ABCDABCDABDE ABCDABDE");
    cout << "String KMP Match: ";
    for (auto i : result)
        cout << i << " ";
    cout << endl;
    cout << "ROT:13 Caeser Cipher: " << str.rotate(13) << endl;

    /**
     * Number Theory
     */

    vector<long long> factors = prime_factorize(144);
    const auto compressed = frequency_compress<long long>(factors);
    cout << "Factors: ";
    for (auto factor : compressed)
        cout << "(" << factor.first << "," << factor.second << ") ";
    cout << "| Expanded out: ";
    for (auto factor : factors)
        cout << factor << " ";
    cout << endl;

    /**
     * Fourier Transforms
     */

    vector<complex<double>> a{1, 2, 3, 4};
    vector<complex<double>> b = fft(a);
    cout << "FFT forward: ";
    for (int i = 0; i < 4; i++)
        cout << b[i] << " ";
    cout << endl;
    cout << "FFT inverse: ";
    vector<complex<double>> c = fft(b, true);
    for (int i = 0; i < 4; i++)
        cout << c[i] << " ";
    cout << endl;
    cout << "NTT forward: ";
    vector<long long> d{1, 2, 3, 4};
    vector<long long> e = ntt(d);
    for (int i = 0; i < 4; i++)
        cout << e[i] << " ";
    cout << endl;
    cout << "NTT inverse: ";
    vector<long long> f = ntt(e, true);
    for (int i = 0; i < 4; i++)
        cout << f[i] << " ";
    cout << endl;
}