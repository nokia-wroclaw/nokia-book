#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

const int INF = 1 << 30;



int main()
{
  vector<int> items;
  time_t start = clock();
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
  
  sort(items.begin(), items.end());

  int minbins = INF;
  int cnt = 0;
  do
  {
    int s = C;
    int r = 1;
    for (int i = 0; i < items.size(); i++)
    {
      s -= items[i];
      if (s < 0)
      {
        s = C - items[i];
        r++;
      }
    }
    minbins = min(minbins, r);
    cnt++;
    if (clock() - start > 15000) goto end;
    
  }while (next_permutation(items.begin(), items.end()));
  cout << "number of permutations" << cnt++ << endl;
  cout << "bins = " << minbins << endl;
end:
  cout << "time = " << clock() - start << endl;

  return 0;
}


