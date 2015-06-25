#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
  vector<int> items;

  int C;
  scanf("%d", &C);
  int n;
  scanf("%d", &n);

  for (int i = 0; i < n; i++)
  {
    int w;
    scanf("%d", &w);
    items.push_back(w);
  }
  
  int nextbin = 0;
  int residual = C;
  for (int i = 0; i < n; i++)
  {
    residual -= items[i];
    if (residual < 0)
    {
      residual = C - items[i];
      nextbin++;
    }
  }
  
  cout << "min bins = " << nextbin + 1 << endl;

  return 0;
}


