#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class MaxPair
{
  vector<int> items;
  int capacity;
  int r1 = -1;
  int r2 = -1;
  int r3 = 1000000;
public:
  MaxPair(int capacity, vector<int> items) 
  {
    this->items = items;
    this->capacity = capacity;
  }
  int operator()()
  {
    
    if (items.size() == 1) return 0;
    
    sort(items.begin(), items.end());
    
    reverse(items.begin(), items.end());
    
    int l = 0;
    int r = items.size() - 1;
    
    int maxS = 0;
    
    pair<int,int> P{-1,-1};
    
    while (l < r)
    {
      if (items[l] + items[r] <= capacity)
      {
        maxS = max(maxS, items[l] + items[r]);
        P = {l, r};
        r--;
      }
      else
      {
        l++;
      }
    }
    
    if (r + 1 < items.size() && items[l] + items[r + 1] <= capacity)
    {
      P = {l, r + 1};
    }

    cout << "maxS " << maxS << " " << P.first << " " << P.second << " l " << l << " r " << r << endl;
    return maxS;
  }
};


int main()
{
  cout << MaxPair(11, {9,8,5,3,3,2,1})();
  cout << MaxPair(13, {9,8,5,4,3,2,1})();
  cout << MaxPair(13, {9,8,5,4,4,2,1})();
  cout << MaxPair(13, {9,8,6,4,3,2,1})();
  cout << MaxPair(13, {9,8,6,4,4,2,1})();
  cout << MaxPair(6, {9,8,6,4,4,4,2,1})();
  cout << MaxPair(6, {9,8,6,4,4,4,1,1,1,1,1})();
  
  return 0;
}


