#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

int main()
{
  int capacity;
  scanf("%d", &capacity);
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
  sort(items.begin(), items.end(), [](int a, int b) { return a > b;} );
  
  for (int i = 0; i < n; i++)
  {
    auto it = S.lower_bound(items[i]);
    if (it == S.end())
      it = S.insert(capacity);
    int el = *it;
    el -= items[i];
    S.erase(it);
    //S.emplace(el);
    S.insert(el);
  }
  
  cout << "min bins = " << S.size() << endl;
  
  return 0;
}

