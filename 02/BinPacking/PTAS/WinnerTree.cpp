#include <iostream>
#include <vector>
#include "WinnerTree.hpp"

using namespace std;

WinnerTree::WinnerTree(int n)
{
  r = 0;
  int i = 1;
  int pow = 0;
  while (i < n)
  {
    i *= 2;
    pow++;
  }
  nodes = 2 * i - 1;
  tree.resize(nodes + 1, 1.0);
  
  p = i;
  cout << "nodes " << nodes << " p " << p << endl;
}

int WinnerTree::winner(double item)
{
  int i = 1;
  while (2 * i <= nodes)
  {
    i *= 2;
    if (tree[i] < item)
      i++;
  }
  
  return i;
}

void WinnerTree::update(int j, double item)
{
  tree[j] -= item;
  r = max(j - p, r);
  while (j > 1)
  {
    j /= 2;
    tree[j] = max(tree[2 * j], tree[2 * j + 1]);
  }
}

int WinnerTree::retp()
{
  return p;
}

int WinnerTree::minBins()
{
  for (int i = 0; i <= nodes; i++  )
  {
    cout << tree[i] << " ";
  }
  cout << " r + 1 " << r + 1 << endl;
  return r + 1;
}


