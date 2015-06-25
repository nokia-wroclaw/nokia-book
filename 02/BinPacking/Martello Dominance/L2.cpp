#include <vector>
#include <algorithm>
#include <iostream>
#include "L2.hpp"
#define DBG(X)
  L2::L2(int C, vector<int> items) 
  {
    this->items = items;
    this->C = C;
  }
  int L2::operator()()
  {
    
    if (items.size() == 1) return 1;
    
    sort(items.begin(), items.end());
    
    reverse(items.begin(), items.end());
    
    vector<int> uni(items);
    
    auto it = unique(uni.begin(), uni.end());
    uni.erase(it, uni.end());
    sort(uni.begin(), uni.end());
    reverse(uni.begin(), uni.end());
    
    find();
    if (jstar == -1) return items.size();
    it = uni.begin();
    while (*it > C / 2)
    {
      ++it;
    }
    
    uni.erase(uni.begin(), it);
    DBG(cout << " C " << C << " *uni.begin() = alfa " << *uni.begin() << " items[1] " << items[1] << "jstar " << jstar << " sJ1 " << sJ1 << " sJ2 " << sJ2 << " sJ3 " << sJ3 << endl;)
    int r = calcL2(*uni.begin());
    DBG(cout << "after calcL2 r " << r << " *uni.begin() = alfa " << *uni.begin() << " items[1] " << items[1] << "jstar " << jstar << " sJ1 " << sJ1 << " sJ2 " << sJ2 << " sJ3 " << sJ3 << endl;)
    
    uni.erase(uni.begin(), uni.begin() + 1);
    
    for (auto x : uni)
    {
      r = max(r, calcNext(x));
    }
    DBG(cout << "r " << r << endl;)
    return r;
  }
  int L2::calcL2(int alfa)
  {
    bool phase2nd = false;
    for (int i = 0; i < items.size(); i++)
    {
      
      if (items[i] > C - alfa)
      {
        J1size++;
        r1 = i;
        r2 = i;
        sJ1 += items[i];
      }
      else
      if (C - alfa >= items[i] && items[i] > C / 2)
      {
        if (!phase2nd)
        {
          r2 = i - 1;
          phase2nd = true;
        }
        J2size++;
        sJ2 += items[i];
      }
      else
      if (C / 2 >= items[i] && items[i] >= alfa)
      {
        J3size++;
        r3 = i + 1;
        sJ3 += items[i];
      }
      DBG(cout << " i " << i << "items[i] " << items[i]<< " sJ1 " << sJ1 << " C " << C << " alfa " << alfa << endl;
      cout << "J1size " << J1size << endl;
      cout << "J2size " << J2size << endl;
      cout << "J3size " << J3size << endl;
      cout << "sJ1 " << sJ1 << endl;
      cout << "sJ2 " << sJ2 << endl;
      cout << "sJ3 " << sJ3 << endl;)
      
    }
    
    int l = (sJ3 - J2size * C + sJ2);
    DBG(cout << "L " << l << endl;)
    int ceilL = (l + C - 1) / C;
    DBG(cout << "ceilL " << ceilL << endl;)
    
    return J1size + J2size + max(0, ceilL);
  }
  int L2::calcNext(int alfa)
  {
    DBG(cout << "r1 " << r1 << " r2 " << r2 << " r3 " << r3 << endl;)
    if (jstar > 0)
    {
      while (r1 >=0 && items[r1] <= C - alfa)
      {
        J1size--;
        sJ1 -= items[r1];
        r1--;
      }
      while (r2 >=0 && items[r2] <= C - alfa)
      {
        J2size++;
        sJ2 += items[r2];
        r2--;
      }
    }
    while (r3 < items.size() && items[r3] >= alfa)
    {
      J3size++;
      sJ3 += items[r3];
      r3++;
    }
    //if (r1 < 0 && r2 < 0 || r3 > items.size()) return -1;

    int l = (sJ3 - J2size * C + sJ2);
    DBG(cout << "L " << l << endl;)
    int ceilL = (l + C - 1) / C;
    
    return J1size + J2size + max(0, ceilL);
  }
  
  void L2::find()
  {
    for (int j = 0; j < items.size(); j++)
    {
      if (items[j] <= C / 2)
      {
        jstar = j;
        break;
      }
    }
    for (int i = jstar; i < items.size(); i++)
    {
      sJ2star += items[i];
    }
  }
  

