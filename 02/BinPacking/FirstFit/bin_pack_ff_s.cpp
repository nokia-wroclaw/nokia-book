#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


int main()
{
  vector<int> items;
  
  vector<int> residual;
  
  int capacity;
  scanf("%d", &capacity);
  int n;
  scanf("%d", &n);
  residual.resize(n, capacity);
  for (int i = 0; i < n; i++)
  {
    int w;
    scanf("%d", &w);
    items.push_back(w);
  }
  
  int r = 0;
  for (int i = 0; i < n; i++)
  {
    int j = 0;
    while (items[i] > residual[j])
    {
      j++;
    }
    r = max(r, j + 1);
    residual[j] -= items[i];
  }
  
  cout << "min bins = " << r << endl;
  
  return 0;
}


