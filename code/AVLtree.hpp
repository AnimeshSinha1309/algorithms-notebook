#ifndef CODE_AVL_TREE_H
#define CODE_AVL_TREE_H

#include <iostream>
#include <vector>

using namespace std;

template <class Type>
class AVLTree
{
  struct Node
  {
    Type data;
    Node *left, *right, *parent;
    int height;
    int subtree_size;
  };

private:
  Node *root = nullptr;

  Node *__insert_node(Node *T, Type x)
  {
    if (T == nullptr)
    {
      T = new Node;
      T->data = x;
      T->left = nullptr;
      T->right = nullptr;
    }
    else if (x > T->data)
    {
      T->right = __insert_node(T->right, x);
      if (__height_difference(T) == -2)
        T = (x > T->right->data) ? __insert_right_right(T) : __insert_right_left(T);
    }
    else if (x < T->data)
    {
      T->left = __insert_node(T->left, x);
      if (__height_difference(T) == 2)
        T = (x < T->left->data) ? insert_left_left(T) : __insert_left_right(T);
    }
    T->height = __height(T);
    T->subtree_size = __subtree_size(T);
    return (T);
  }

  Node *__delete_node(Node *T, Type x)
  {
    Node *p;
    if (T == nullptr)
    { // Delete nothing
      return nullptr;
    }
    else if (x > T->data)
    { // Go to the right to find entity to delete and balance
      T->right = __delete_node(T->right, x);
      if (__height_difference(T) == 2)
        T = (__height_difference(T->left) >= 0) ? insert_left_left(T) : __insert_left_right(T);
    }
    else if (x < T->data)
    { // Go to the left to find entity to delete and balance
      T->left = __delete_node(T->left, x);
      if (__height_difference(T) == -2)
        T = (__height_difference(T->right) <= 0) ? __insert_right_right(T) : __insert_right_left(T);
    }
    else
    { // Delete the actual node
      if (T->right != nullptr)
      {
        p = T->right;
        while (p->left != nullptr)
          p = p->left;
        T->data = p->data;
        T->right = __delete_node(T->right, p->data);
        if (__height_difference(T) == 2)
          T = (__height_difference(T->left) >= 0) ? insert_left_left(T) : __insert_left_right(T);
      }
      else
        return (T->left);
    }
    T->height = __height(T);
    T->subtree_size = __subtree_size(T);
    return (T);
  }

  Node *__search_node(Node *T, Type x)
  {
    if (T == nullptr || T->data == x)
    {
      return T;
    }
    else if (x < T->data)
    {
      return __search_node(T->left, x);
    }
    else if (x > T->data)
    {
      return __search_node(T->right, x);
    }
  }
  unsigned long __count_lt(Node *T, Type x)
  {
    if (T == nullptr)
    {
      return 0;
    }
    else if (x <= T->data)
    {
      return __count_lt(T->left, x);
    }
    else if (x > T->data)
    {
      return __count_lt(T->right, x) + __subtree_size(T->left) + 1;
    }
    return 0;
  }
  unsigned long __count_gt(Node *T, Type x)
  {
    if (T == nullptr)
    {
      return 0;
    }
    else if (x < T->data)
    {
      return __count_gt(T->left, x) + __subtree_size(T->right) + 1;
    }
    else if (x >= T->data)
    {
      return __count_gt(T->right, x);
    }
    return 0;
  }
  Node *__search_quantile(Node *T, unsigned long count)
  {
    if (T == nullptr || count == 1)
    {
      return T;
    }
    else if (count < __subtree_size(T->left))
    {
      return __search_quantile(T->left, count);
    }
    else if (count == __subtree_size(T->left))
    {
      return T;
    }
    else
    {
      return __search_quantile(T->right, count - __subtree_size(T->left) - 1);
    }
  }

  int __height(Node *T)
  {
    int left_height, right_height;
    if (T == nullptr)
      return (0);
    left_height = (T->left == nullptr) ? 0 : 1 + T->left->height;
    right_height = (T->right == nullptr) ? 0 : 1 + T->right->height;
    return max(left_height, right_height);
  }
  int __subtree_size(Node *T)
  {
    int left_size, right_size;
    if (T == nullptr)
      return (0);
    left_size = (T->left == nullptr) ? 0 : T->left->subtree_size;
    right_size = (T->right == nullptr) ? 0 : T->right->subtree_size;
    return left_size + right_size + 1;
  }

  Node *__rotate_right(Node *x)
  {
    Node *y;
    y = x->left;
    x->left = y->right;
    y->right = x;
    x->height = __height(x);
    y->height = __height(y);
    x->subtree_size = __subtree_size(x);
    y->subtree_size = __subtree_size(y);
    return (y);
  }

  Node *__rotate_left(Node *x)
  {
    Node *y;
    y = x->right;
    x->right = y->left;
    y->left = x;
    x->height = __height(x);
    y->height = __height(y);
    x->subtree_size = __subtree_size(x);
    y->subtree_size = __subtree_size(y);
    return (y);
  }

  Node *__insert_right_right(Node *T)
  {
    T = __rotate_left(T);
    return (T);
  }
  Node *insert_left_left(Node *T)
  {
    T = __rotate_right(T);
    return (T);
  }
  Node *__insert_left_right(Node *T)
  {
    T->left = __rotate_left(T->left);
    T = __rotate_right(T);
    return (T);
  }
  Node *__insert_right_left(Node *T)
  {
    T->right = __rotate_right(T->right);
    T = __rotate_left(T);
    return (T);
  }

  int __height_difference(Node *T)
  {
    int lh, rh;
    if (T == nullptr)
      return (0);
    lh = (T->left == nullptr) ? 0 : 1 + T->left->height;
    rh = (T->right == nullptr) ? 0 : 1 + T->right->height;
    return (lh - rh);
  }

public:
  AVLTree()
  {
    root = nullptr;
  }
  explicit AVLTree(vector<Type> data)
  {
    root = nullptr;
    for (auto el : data)
      root = __insert_node(root, el);
  }

  void in_order_traversal(Node *T)
  {
    if (T != nullptr)
    {
      in_order_traversal(T->left);
      cout << T->data << " ";
      in_order_traversal(T->right);
    }
  }
  void print() { in_order_traversal(root); }

  void insert(Type x)
  {
    root = __insert_node(root, x);
  }
  void erase(Type x)
  {
    root = __delete_node(root, x);
  }
  Node *search(Type x)
  {
    return __search_node(root, x);
  }

  unsigned long count_lt(Type x)
  {
    return __count_lt(root, x);
  }
  unsigned long count_gt(Type x)
  {
    return __count_gt(root, x);
  }
  unsigned long count_eq(Type x)
  {
    return (root != nullptr ? root->subtree_size : 0) - __count_lt(root, x) - __count_gt(root, x);
  }
  unsigned long count_le(Type x)
  {
    return (root != nullptr ? root->subtree_size : 0) - __count_gt(root, x);
  }
  unsigned long count_ge(Type x)
  {
    return (root != nullptr ? root->subtree_size : 0) - __count_lt(root, x);
  }
  Node *search_quantile(unsigned long index)
  {
    return __search_quantile(root, index);
  }

  void size()
  {
    return root->subtree_size;
  }
};

#endif //CODE_AVL_TREE_H