#include <vector>
#include <iostream>
#include "L1.hpp"

L1::L1(int capacity, vector<int> items) 
{
  this->items = items;
  this->capacity = capacity;
}
int L1::operator()()
{
  int s = 0;
  for (int i = 0; i < items.size(); i++)
    s += items[i];
  int L = (s + capacity - 1) / capacity;
  return L;
}

