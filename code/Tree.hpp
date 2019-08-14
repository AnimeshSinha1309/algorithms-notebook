#ifndef CODE_TREE_H
#define CODE_TREE_H

#include <iostream>
#include <vector>
using namespace std;

template <class Type>
class Tree
{
public:
  struct Node
  {
    Type data;
    vector<Node *> adjacent;
    Node *parent = nullptr;
    long long start_time = 0, end_time = 0;
  };

  vector<Node> list;
  Node *root = nullptr;

  explicit Tree(vector<Type> data)
  {
    list.reserve(data.size());
    for (auto datum : data)
    {
      list.emplace_back(datum);
    }
  }
  Tree(vector<Type> data, vector<pair<int, int>> edges)
  {
    list.reserve(data.size());
    for (auto datum : data)
    {
      list.emplace_back(datum);
    }
    for (auto edge : edges)
    {
      add_edge(edge.first, edge.second);
    }
    __build();
  }
  void add_edge(int x, int y)
  {
    list[x].adjacent.push_back(&list[y]);
    list[y].adjacent.push_back(&list[x]);
  }

  int __build(Node *root = nullptr, int time = 0)
  {
    if (root == nullptr)
      root = this->root;
    root->start_time = time;
    for (auto child : root->adjacent)
    {
      if (child == root->parent)
        continue;
      child->parent = root;
      time = __build(child, time + 1);
    }
    root->end_time = time;
    return time;
  }
};

#endif // CODE_TREE_H
