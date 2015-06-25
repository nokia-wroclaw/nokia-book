#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
#include <tuple>
#include <functional>
#include "bin_pack_dyn4.hpp"
using namespace std;


namespace dyn4
{


static   int A[14][14][21][14];
  static int B[14][14][21][14];
  static X C[14][14][21][14];
  

dyn4::dyn4(vector<int> S, vector<int> x, int capacity)
{
  this->S = S;
  this->x = x;
  this->capacity = capacity;
}


void dyn4::calcPath(X cur, X pop)
{
  X sol = {cur.a - pop.a, cur.b - pop.b,cur.c - pop.c,cur.d - pop.d};

  //if (sol.a + sol.b + sol.c + sol.d == 0) 
  if (cur.a + cur.b + cur.c + cur.d == 0) 
  {
    return;
  }

  calcPath(sol, C[sol.a][sol.b][sol.c][sol.d]);
  
  bins.push_back(S[0] * pop.a + S[1] * pop.b + S[2] * pop.c + S[3] * pop.d);

  //cout << sol.a << " " << sol.b << " " << sol.c << " " << sol.d << endl;
  cout << pop.a << " " << pop.b << " " << pop.c << " " << pop.d << endl;
}

void dyn4::prepare_conf()
{
  for (int i = 0; i <= x[0]; i++)
    for (int j = 0; j <= x[1]; j++)
      for (int k = 0; k <= x[2]; k++)
        for (int l = 0; l <= x[3]; l++)
        {
          if (i + j + k + l == 0) continue;
          if (S[0] * i + S[1] * j + S[2] * k + S[3] * l <= capacity)
          {
            A[i][j][k][l] = 1;
            C[i][j][k][l] = {0,0,0,0};
            CONF.push_back(make_tuple(i,j,k,l));
          }
        }
}

vector<int> dyn4::dp()
{
  prepare_conf();
  A[0][0][0][0] = 0;
  for (int i = 0; i <= x[0]; i++)
    for (int j = 0; j <= x[1]; j++)
      for (int k = 0; k <= x[2]; k++)
        for (int l = 0; l <= x[3]; l++)
        {
          int cur = 1 << 30;
          for (auto& c : CONF)
          {
            int x1 = get<0>(c), x2 = get<1>(c);
            int x3 = get<2>(c), x4 = get<3>(c);
            if (i >= x1 && j >= x2 && k >= x3 && l >= x4)
            {
              if (cur > A[i - x1][j - x2][k - x3][l - x4] + 1)
              {
                C[i][j][k][l] = {x1, x2, x3, x4};
                cur = A[i - x1][j - x2][k - x3][l - x4] + 1;
              }
              //cur = min(cur, A[i - x1][j - x2][k - x3][l - x4] + 1);
            }
             
          }
          if (cur < (1 << 30)) 
            A[i][j][k][l] = cur;
        }

          for (int c = 0; c < CONF.size(); c++)
          {
            int x1 = get<0>(CONF[c]);
            int x2 = get<1>(CONF[c]);
            int x3 = get<2>(CONF[c]);
            int x4 = get<3>(CONF[c]);
            
            cout << x1 << " " << x2 << " " << x3 << " " << x4 << endl;
          }

  for (int i = 0; i <= x[0]; i++)
    for (int j = 0; j <= x[1]; j++)
      for (int k = 0; k <= x[2]; k++)
        for (int l = 0; l <= x[3]; l++)
  
          cout << i << " " << j << " " << k <<" " << l << " " << A[i][j][k][l] << endl;
          
  cout << A[x[0]][x[1]][x[2]][x[3]] << endl;

  X pop = C[x[0]][x[1]][x[2]][x[3]];
  X cur = {x[0], x[1], x[2], x[3]};

  cout << "path" << endl;
  calcPath(cur, {0,0,0,0});
  cout << "path end" << endl;

  cout << cur.a << " " << cur.b << " " << cur.c << " " << cur.d << endl;
  cout << pop.a << " " << pop.b << " " << pop.c << " " << pop.d << endl;

  while (1)
  { 
    X sol = {cur.a - pop.a, cur.b - pop.b,cur.c - pop.c,cur.d - pop.d};
    cout << sol.a << " " << sol.b << " " << sol.c << " " << sol.d << endl;
    if (sol.a + sol.b + sol.c + sol.d == 0) break;
    
    pop = C[sol.a][sol.b][sol.c][sol.d];
    cur = sol;
  }
  
  bins.pop_back();

  for (int i = 0; i < bins.size(); i++)
  {
    cout << "bin " << i << " " << bins[i] << " ";
  }
  return bins;
}

}
