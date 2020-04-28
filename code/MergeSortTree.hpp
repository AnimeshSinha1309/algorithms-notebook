#ifndef CODE_MERGESORT_H
#define CODE_MERGESORT_H

#include "../ref/template.hpp"

template <typename Type>
struct MergeSortTree {
    int size;
    vector<Type> data;
    vector<vector<int>> tree_idx;
    vector<vector<Type>> tree_val;
    long long inversions;

    template <typename DataType>
    vector<DataType> merge(const vector<DataType> &arr1,
                           const vector<DataType> &arr2) {
        int n = arr1.size(), m = arr2.size();
        vector<DataType> result;
        result.reserve(n + m);
        for (int x = 0, y = 0; x < n || y < m;) {
            if (x < n && (y >= m || arr1[x] <= arr2[y]))
                result.push_back(arr1[x++]);
            else
                result.push_back(arr2[y++]), inversions += n - x;
        }
        return move(result);
    }
    int order_fn(const Type &value, const vector<Type> &arr) {
        return lower_bound(arr.begin(), arr.end(), value) - arr.begin();
    }
    explicit MergeSortTree(const vector<Type> &list) {
        for (size = 1; size < list.size(); size *= 2)
            ;
        // Make a tree based on the values
        tree_val.resize(2 * size);
        data = vector<Type>(list);
        for (int i = 0; i < list.size(); i++)
            tree_val[i + size].push_back(i);
        for (int i = size - 1; i > 0; --i)
            tree_val[i] = merge<Type>(tree_val[i << 1], tree_val[i << 1 | 1]);
        // Make a tree based on the indices
        tree_idx.resize(2 * size);
        vector<pair<Type, int>> convert(list.size());
        for (int i = 0; i < list.size(); i++)
            convert[i].first = list[i], convert[i].second = i;
        sort(convert.begin(), convert.end());
        for (int i = 0; i < list.size(); i++)
            tree_idx[i + size].push_back(convert[i].second);
        for (int i = size - 1; i > 0; --i)
            tree_idx[i] = merge<int>(tree_idx[i << 1], tree_idx[i << 1 | 1]);
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
        int m = (upper_bound(tree_idx[2 * node].begin(),
                             tree_idx[2 * node].end(), r - 1) -
                 tree_idx[2 * node].begin()) -
                (lower_bound(tree_idx[2 * node].begin(),
                             tree_idx[2 * node].end(), l) -
                 tree_idx[2 * node].begin());
        if (m >= order)
            return key_of_order(l, r, order, node << 1, x, (x + y) / 2);
        else
            return key_of_order(l, r, order - m, node << 1 | 1, (x + y) / 2, y);
    }
};

long long __inversions = 0;
void __merge(int *A, int start, int mid, int end) {
    int result[end - start];
    for (int x = start, y = mid; x < mid || y < end;) {
        if (x < mid && (y >= end || A[x] <= A[y])) {
            result[x + y - start - mid] = A[x];
            x++;
        } else {
            result[x + y - start - mid] = A[y];
            y++;
            __inversions += mid - x;
        }
    }
    for (int i = start; i < end; i++)
        A[i] = result[i - start];
}
void sort(int A[], int start, int end) {
    if (start >= end - 1)
        return;
    sort(A, start, (start + end) / 2);
    sort(A, (start + end) / 2, end);
    __merge(A, start, (start + end) / 2, end);
}

#endif