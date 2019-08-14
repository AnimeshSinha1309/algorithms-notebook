#ifndef CODE_SEGUPD_H
#define CODE_SEGUPD_H

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
using namespace std;

template <class Type>
class SegmentUpdate
{
protected:
  vector<Type> data;
  unsigned long size;
  inline unsigned long parent(unsigned long i) { return i >> 1; }
  inline unsigned long lChild(unsigned long i) { return i << 1; }
  inline unsigned long rChild(unsigned long i) { return i << 1 | 1; }
  inline unsigned long sibling(unsigned long i) { return i ^ 1; }
  inline unsigned long element(unsigned long i) { return i + size; }
  inline bool isRoot(unsigned long i) { return i == 1; }
  inline bool islChild(unsigned long i) { return (i & 1) == 0; }
  inline bool isrChild(unsigned long i) { return (i & 1) != 0; }
  function<Type(Type, Type)> operation;
  Type defaultValue;

public:
  explicit SegmentUpdate(const vector<Type> &list, function<Type(Type, Type)> segOperation, Type defaultTo)
  {
    size = (1ul << (long)ceil(log2(list.size())));
    data = vector<Type>(size * 2, defaultTo);
    defaultValue = defaultTo;
    operation = segOperation;
    for (unsigned long i = 0; i < list.size(); i++)
      data[i + size] = list[i];
    for (unsigned long i = size - 1; i > 0; --i)
      data[i] = operation(data[lChild(i)], data[rChild(i)]);
  }
  void modify(unsigned long l, unsigned long r, Type value)
  {
    for (l = element(l), r = element(r); l < r; l = parent(l), r = parent(r))
    {
      if (isrChild(l))
      {
        data[l] = operation(data[l], value);
        l++;
      }
      if (isrChild(r))
      {
        --r;
        data[r] = operation(data[r], value);
      }
    }
  }
  Type query(unsigned long position)
  {
    Type accumulator = defaultValue;
    for (position = element(position);; position = parent(position))
    {
      accumulator = operation(accumulator, data[position]);
      if (isRoot(position))
        break;
    }
    return accumulator;
  }
};

#endif //CODE_SEGUPD_H
