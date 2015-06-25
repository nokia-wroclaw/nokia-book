#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
#include <tuple>
#include <functional>

#include "bin_pack_dyn3.hpp"

#define DBG(X)

using namespace std;

namespace dyn3
{

static int A[361][361][364];
static int B[361][361][364];
static X C[364][364][364];


dyn3::dyn3(vector<double> S, vector<int> x, int capacity)
{
  this->S = S;
  this->x = x;
  this->capacity = capacity;
}


void dyn3::calcPath(X cur, X pop)
{
  X sol = {cur.a - pop.a, cur.b - pop.b,cur.c - pop.c};

  if (cur.a + cur.b + cur.c == 0) 
  {
    return;
  }

  calcPath(sol, C[sol.a][sol.b][sol.c]);
  
  bins.push_back(1.0 - (S[0] * pop.a + S[1] * pop.b + S[2] * pop.c));

  DBG(cout << pop.a << " " << pop.b << " " << pop.c << " " << endl;)
}

void dyn3::prepare_conf()
{
  for (int i = 0; i <= x[0]; i++)
    for (int j = 0; j <= x[1]; j++)
      for (int k = 0; k <= x[2]; k++)
        {
          if (i + j + k  == 0) continue;
          if (S[0] * i + S[1] * j + S[2] * k <= capacity)
          {
            A[i][j][k] = 1;
            C[i][j][k] = {0,0,0};
            CONF.push_back(make_tuple(i,j,k));
          }
        }
}


vector<double> dyn3::dp()
{
  prepare_conf();
  A[0][0][0] = 0;
  for (int i = 0; i <= x[0]; i++)
    for (int j = 0; j <= x[1]; j++)
      for (int k = 0; k <= x[2]; k++)
        {
          int cur = 1 << 30;
          for (auto& c : CONF)
          {
            int x1 = get<0>(c), x2 = get<1>(c);
            int x3 = get<2>(c);
            if (i >= x1 && j >= x2 && k >= x3)
            {
              if (cur > A[i - x1][j - x2][k - x3]+ 1)
              {
                C[i][j][k] = {x1, x2, x3};
                cur = A[i - x1][j - x2][k - x3] + 1;
              }
            }
          }
          if (cur < (1 << 30)) 
            A[i][j][k] = cur;
        }

          DBG(for (int c = 0; c < CONF.size(); c++)
          {
            int x1 = get<0>(CONF[c]);
            int x2 = get<1>(CONF[c]);
            int x3 = get<2>(CONF[c]);
            
            cout << x1 << " " << x2 << " " << x3 << endl;
          })

  DBG(for (int i = 0; i <= x[0]; i++)
    for (int j = 0; j <= x[1]; j++)
      for (int k = 0; k <= x[2]; k++)
  
          cout << i << " " << j << " " << k <<" " << A[i][j][k] << endl;
          
  cout << A[x[0]][x[1]][x[2]] << endl;)

  X pop = C[x[0]][x[1]][x[2]];
  X cur = {x[0], x[1], x[2] };

  DBG(cout << "path" << endl;)
  calcPath(cur, {0,0,0});
  DBG(cout << "path end" << endl;)

  DBG(cout << cur.a << " " << cur.b << " " << cur.c << " " << endl;
  cout << pop.a << " " << pop.b << " " << pop.c << " " << endl;)

  while (1)
  { 
    X sol = {cur.a - pop.a, cur.b - pop.b,cur.c - pop.c};
    DBG(cout << sol.a << " " << sol.b << " " << sol.c << " "  << endl;)
    if (sol.a + sol.b + sol.c == 0) break;
    
    pop = C[sol.a][sol.b][sol.c];
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

