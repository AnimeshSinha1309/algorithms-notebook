#include "template.hpp"

template <typename Type>
class MergeSortTree {
   protected:
    int size;
    vector<Type> data;
    vector<vector<int>> tree_idx;
    vector<vector<Type>> tree_val;
    long long inversions;

    vector<pair<int, Type>> merge(const vector<int> &arr1,
                                  const vector<int> &arr2) {
        int n = arr1.size(), m = arr2.size();
        vector<pair<int, Type>> result(n + m);
        for (int x = 0, y = 0; x < n || y < m;) {
            if (x < n && (y >= m || data[arr1[x]] <= data[arr2[y]]))
                result.push_back(arr1[x++]);
            else
                result.push_back(arr2[y++]), inversions += n - x;
        }
        return move(result);
    }

    int order_fn(const Type &value, const vector<Type> &arr) {
        return lower_bound(arr.begin(), arr.end(), value) - arr.begin();
    }

   public:
    explicit MergeSortTree(const vector<Type> &list) {
        for (size = 1; size < list.size(); size *= 2)
            ;
        tree_idx.resize(2 * size);
        tree_val.resize(2 * size);
        for (int i = 0; i < list.size(); i++)
            tree_idx[i + size].emplace_back(i, list[i]);
        for (int i = size - 1; i > 0; --i)
            tree_idx[i] = merge(tree[i << 1], tree[i << 1 | 1]);
        for (int i = 0; i < 2 * size; i++)
            for (int el : tree_idx[i])
                tree_val[i].push_back(data[el]);
    }

    int order_of_key(int l, int r, Type value) {
        int result = 0;
        for (l = l + size, r = r + size; l < r; l >>= 1, r >>= 1) {
            if (l & 1)
                result += order_fn(value, tree_val[l++]);
            if (r & 1)
                result += order_fn(value, tree_val[--r]);
        }
        return result;
    }

    int key_of_order(int l, int r, int order, int node = 0, int x = 0,
                     int y = -1) {
        if (y == -1)
            y = size;
        if (x + 1 == y)
            return tree_idx[node][0];
        int last_in_query_range = upper_bound(tree_idx[2 * node].begin(),
                                              tree_idx[2 * node].end(), r - 1) -
                                  tree_idx[2 * node].begin();
        int first_in_query_range = lower_bound(tree_idx[2 * node].begin(),
                                               tree_idx[2 * node].end(), l) -
                                  tree_idx[2 * node].begin());
        int m = last_in_query_range - first_in_query_range;
        if (m >= k)
            return key_of_order(l, r, order, node << 1, x, (x + y) / 2);
        else
            return key_of_order(l, r, order - m, node << 1 | 1, (x + y) / 2, y);
    }
};
