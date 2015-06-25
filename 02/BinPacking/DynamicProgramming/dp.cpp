#include <vector>

#include "bin_pack_dyn2.hpp"
#include "bin_pack_dyn3.hpp"
#include "bin_pack_dyn4.hpp"

int main()
{
  int C;
  scanf("%d", &C);
  int r;
  scanf("%d", &r);
  
  vector<int> items;
  vector<int> repetitions;
  
  for (int i = 0; i < r; i++)
  {
    int w;
    scanf("%d", &w);
    items.push_back(w);
  }

  for (int i = 0; i < r; i++)
  {
    int z;
    scanf("%d", &z);
    repetitions.push_back(z);
  }
  

  if (r == 4)
  {
    dyn4::dyn4 d4a(items, repetitions, C);
    vector<int> bins = d4a.dp();
  }
  else if (r == 3)
  {
    dyn3::dyn3 d3a(items, repetitions, C);
    vector<int> bins = d3a.dp();
  }
  else if (r == 2)
  {
    dyn2::dyn2 d2a(items, repetitions, C);
    vector<int> bins = d2a.dp();
  }
  

  return 0;
  
}

