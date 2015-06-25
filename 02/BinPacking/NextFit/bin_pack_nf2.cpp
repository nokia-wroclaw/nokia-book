#include <cstdio>
#include <iostream>

using namespace std;

int main()
{
  int C;
  scanf("%d", &C);
  int n;
  scanf("%d", &n);
  
  int residual = C;
  int nextbin = 0;
  for (int i = 0; i < n; i++)
  {
    int w;
    scanf("%d", &w);
    residual -= w;
    if (residual < 0)
    {
      residual = C - w;
      nextbin++;
    }
  }
  
  cout << "min bins = " << nextbin + 1 << endl;

  return 0;
}


