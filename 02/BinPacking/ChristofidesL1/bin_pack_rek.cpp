#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <ctime>
#include "L1.hpp"

using namespace std;

const int INF = 1 << 30;

vector<int> items;
vector<int> visited;
vector<int> residual;
vector<int> itemscontain;
int C ;

int nodes = 0;
int bestSol = INF;
int bestL1 = INF;
int UpperBound = INF;
time_t start;

int packItem(int index)
{
  int r = INF;
  if (clock() - start > 15000) return -1;
  
  vector<pair<int,int>> res;
  for (int i = 0; i < residual.size(); i++)
  {
    if (residual[i] < C) res.push_back({residual[i], i});
  }
  int b = 0;
  for (int i = 0; i < residual.size(); i++)
  {
    if (residual[i] == C) 
    {
      b = i; 
      break;
    }
  }
  sort(res.begin(), res.end());
  res.push_back({C, b});

  int jstar = 0;
  for (; jstar < items.size(); jstar++)
  {
    if (itemscontain[jstar]) break;
  }
  
  int s = 0;
  int L1inNode = 0;
  vector<int> elemsInNode;
  
  for (int i = 0; i < items.size(); i++)
  {
    if (itemscontain[i])
    {
      elemsInNode.push_back(items[i]);
    }
  }
  
  for (int i = 0; i < residual.size(); i++)
  {
    s += residual[i] < C;
    if (residual[i] < C)
      elemsInNode.push_back(C - residual[i]);
  }
  
  if (jstar == items.size())
  {
    bestSol = min(bestSol, s);
    UpperBound = min(UpperBound, s);
    return bestSol;
  }
  
  if (UpperBound != INF && s > UpperBound - 1)
  {
    return s;
  }  
  if (s)
  {
    L1inNode = L1(C, elemsInNode)();
    if (bestSol != INF && L1inNode >= bestSol)
    {
      cout << "fathom L1" << endl;
      return bestSol;
    }
  }
  
  if (s >= bestSol)
  {
    return s;
  }

  if (bestSol == bestL1)
  {
    return bestSol;
  }

  int h = 0;
  for (; h < res.size(); h++)
  {
    if (items[jstar] <= res[h].first) break;
  }
  
  for (int i = h; i < res.size(); i++)
  {
    if (!visited[jstar] && residual[res[i].second] >= items[jstar])
    {
      if (residual[res[i].second] == C)
      {
        if (s >= UpperBound - 1) continue;
      }
      
      residual[res[i].second] -= items[jstar];
      visited[jstar] = true;
      itemscontain[jstar] = false;
      nodes++;
      int next = 0;
      for (; next < res.size(); next++)
        if (!visited[next]) break;
      r = min(packItem(next), r);
      visited[jstar] = false;
      residual[res[i].second] += items[jstar];
      itemscontain[jstar] = true;
    }
  }
  
  return r;
}

int main()
{
  start = clock();
  scanf("%d", &C);
  int n;
  scanf("%d", &n);
  visited.resize(n, 0);
  residual.resize(n, C);
  itemscontain.resize(n, 1);
  
  for (int i = 0; i < n; i++)
  {
    int w;
    scanf("%d", &w);
    items.push_back(w);
  }
  sort(items.begin(), items.end());
  reverse(items.begin(), items.end());
    
  
  bestL1 = L1(C, items)();
  
  
  cout << "min bins = " << packItem(0) << endl;
  cout << "nr of visited nodes " << nodes;
end:
  cout << "time = " << clock() - start << endl;

  return 0;
}


