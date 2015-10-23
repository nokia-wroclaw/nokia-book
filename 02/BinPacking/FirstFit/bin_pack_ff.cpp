  #include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class WinnerTree
{
  int nodes;
  int p;
  int r;
  vector<int> tree;
public:
  WinnerTree(int n, int capacity)
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
    tree.resize(nodes + 1, capacity);
    
    p = i;
  }
  
  int winner(int item)
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
  
  void update(int j, int item)
  {
    tree[j] -= item;
    r = max(j - p, r);
    while (j > 1)
    {
      j /= 2;
      tree[j] = max(tree[2 * j], tree[2 * j + 1]);
    }
  }
  
  int minBins()
  {
    return r + 1;
  }

};

int main()
{
  vector<int> items;
  int capacity;
  scanf("%d", &capacity);
  int n;
  scanf("%d", &n);
  for (int i = 0; i < n; i++)
  {
    int w;
    scanf("%d", &w);
    items.push_back(w);
  }
  
  WinnerTree tree(n, capacity);
  tree.minBins();

  for (int i = 0; i < n; i++)
  {
    int win = tree.winner(items[i]);
    tree.update(win, items[i]);
    tree.minBins();
  }
  
  cout << "min bins = " << tree.minBins() << endl;
  
  return 0;
}

