#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "bin_pack_dyn2.hpp"
#include "bin_pack_dyn3.hpp"
#include "bin_pack_dyn4.hpp"
#include "WinnerTree.hpp"

using namespace std;

int main()
{
  //double eps = 0.58; //gr = 5 l = 7
  //double eps = 0.48;//gr = 5 l = 12
  //double eps = 0.38;//gr = 5 l = 12
  
  //double eps = 0.3;
  
  //test0 //gr = 4 l = 30
  //test1 //gr = 4 l = 5
  double eps = 0.77;
  
  //test2 //gr = 4 l = 5
  //test5 eps 288.646 l 289 largesize 1000 gr 4 smallsize 500 45.36
  //test6 eps 1443.23 l 1444 largesize 5000 gr 4 smallsize 2500 45.36
  //double eps = 0.68;
  
  //test5 eps 223.438 l 224 largesize 500 gr 3 smallsize 1000 50.672
  //test6 eps 1117.19 l 1118 largesize 2500 gr 3 smallsize 5000 50.76 r 3354
  //test7 eps 28.27 l 29 largesize 64 gr 3 smallsize 256 50.76 r 113
  //double eps = 0.88;
  
  //test10 eps 9.5 l 10 largesize 50 gr 5 smallsize 0 37.26 r 30
  //test11 eps 1.9 l 2 largesize 10 gr 5 smallsize 0 37.26 r 6
  //test12 eps 5.7 l 6 largesize 30 gr 5 smallsize 0 37.26 r 18
  //testrandom eps 8.6184 l 9 largesize 39 gr 5 smallsize 9 37.26 r 32
  //pack8 eps 19954.4 l 19955 largesize 238720 gr 12 smallsize 761280 37.26
  //double eps = 0.38;
  
  
  //double eps = 0.88;
  
  
  int C;
  scanf("%d", &C);
  int n;
  scanf("%d", &n);
  
  vector<double> large;
  vector<double> small;
  
  double s = 0.0;
  for (int i = 0; i < n; i++)
  {
    int w;
    scanf("%d", &w);
//    cout << w << endl;
    double x = w * 1.0 / C;
    cout << "w " << w << " x " << x << endl;
    
    if (x <= eps / 2.0) 
    {
      small.push_back(x);
    }
    else
    {
      large.push_back(x);
      s += x;
    }
  }
  
  int l = ceil(eps * s);
  
  
  sort(large.begin(), large.end());
  reverse(large.begin(), large.end());
  
  int gr = (large.size() + l - 1) / l;
  
  cout << "eps " << eps * s << " l " << l << " largesize " << large.size() << " gr " << gr << " smallsize " << small.size() << " " << 27 *(1+eps)<< endl;
  vector<double> G;
  vector<int> x;
  int k = 0;
  for (int i = 1; i < gr; i++)
  {
    G.push_back(large[l * i]);
    x.push_back(l);
  }

  vector<double> residual;
  cout << " gr " << gr << " l " << l << endl;
  gr--;
  if (gr == 4)
  {
    dyn4::dyn4 d4(G, x, 1.0);
    residual = d4.dp();
  }
  else if (gr == 3)
  {
    dyn3::dyn3 d3(G, x, 1.0);
    residual = d3.dp();
  }
  else if (gr == 2)
  {
    dyn2::dyn2 d2(G, x, 1.0);
    residual = d2.dp();
  }
  
  sort(residual.begin(), residual.end(), [](double a, double b) { return a < b;});
  
  WinnerTree tree(residual.size() + l + small.size());
  
  for (int i = 0; i < residual.size(); i++)
  {
    tree.update(tree.retp() + i, 1.0 - residual[i]);
    //tree.minBins();
  }
  
  for (int i = 0; i < l; i++)
  {
    int win = tree.winner(large[i]);
    //cout << "win " << win << " " << large[i] << endl;
    tree.update(win, large[i]);
    
  }
  tree.minBins();
  
  for (int i = 0; i < small.size(); i++)
  {
    int win = tree.winner(small[i]);
    //cout << "win " << win << " small " << small[i] << endl;
    tree.update(win, small[i]);
    
  }
  cout << tree.minBins() << endl;


  
  //for (int i = 0; i < residual.size(); i++)
  //{
    //cout << " i " << i << " " << residual[i] << endl;
  //}
  
  
  
  return 0;
}

