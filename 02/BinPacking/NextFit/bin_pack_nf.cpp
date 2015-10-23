#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

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
  
  int nextbin = 0;
  int residual = capacity;
  for (int i = 0; i < n; i++)
  {
    residual -= items[i];
    if (residual < 0)
    {
      residual = capacity - items[i];
      nextbin++;
    }
  }
  
  cout << "min bins = " << nextbin + 1 << endl;

  return 0;
}


