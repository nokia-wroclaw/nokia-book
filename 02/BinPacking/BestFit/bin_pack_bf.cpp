#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

int main()
{
  int C;
  scanf("%d", &C);
  int n;
  scanf("%d", &n);
  
  multiset<int> S;
  vector<int> items;
  for (int i = 0; i < n; i++)
  {
    int w;
    scanf("%d", &w);
    items.push_back(w);
  }
 
  for (auto const& item : items)
  {
    auto it = S.lower_bound(item);
    if (it == S.end())
      it = S.insert(C);
    int el = *it;
    el -= item;
    S.erase(it);
    S.emplace(el);
  }
  
  cout << "min bins = " << S.size() << endl;
  
  return 0;
}

