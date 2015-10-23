#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <ctime>
#include "L2.hpp"
using namespace std;
#define DBG(X)

vector<int> items;
vector<int> residual;
vector<vector<int>> has;
vector<int> visited;
const int INF = 1 << 30;
int UpperBound = INF;

int capacity;
int nodes = 0;
int bestSol = INF;
int bestL2 = INF;
time_t start;

void printSolution()
{
  for (int i = 0; i < residual.size(); i++)
  {
    cout << "Bin " << i << " has items: ";
    for (int j = 0; j < items.size(); j++)
    {
      if (has[i][j])
      {
        cout << items[j] << " ";
      }
    }
    cout << endl;
  }
}

int packItem(int index)
{
  if (clock() - start > 15000) return -1;
  int r = INF;
  
  int s = 0;
  int L2inNode = 0;
  vector<int> elemsInNode;
  for (int i = 0; i < items.size(); i++)
  {
    if (!visited[i])
    {
      elemsInNode.push_back(items[i]);
    }
  }
  for (int i = 0; i < residual.size(); i++)
  {
    s += residual[i] < capacity;
    if (residual[i] < capacity)
      elemsInNode.push_back(capacity - residual[i]);
  }
  //printSolution();
  if (index == items.size())
  {
    
    bestSol = min(bestSol, s);
    UpperBound = min(UpperBound, s);
  }
  if (UpperBound != INF && s > UpperBound - 1)
  {
    return s;
  }    
  if (s)
  {
    L2inNode = L2(capacity, elemsInNode)();
    DBG(cout << "L2inNode " << L2inNode << " bestSol " << bestSol << endl;)
    if (bestSol != INF && L2inNode >= bestSol)
    {
      DBG(cout << "L2 cut" << endl;)
      return bestSol;
    }
  }
  if (s >= bestSol)
  {
    return bestSol;
  }

  DBG(cout << "bestSol " << bestSol << " bestL2 " << bestL2 << endl;)
  if (bestSol == bestL2)
  {
    return bestSol;
  }
  
  for (int i = 0; i < residual.size(); i++)
  {
    if (!visited[index] && residual[i] >= items[index])
    {
      if (residual[i] == capacity)
      {
        if (s >= UpperBound - 1) continue;
      }
      
      DBG(cout << "bin not closed" << i << endl;)
      DBG(cout << "packed item index " << index << endl;)
      residual[i] -= items[index];
      has[i][index] = true;
      visited[index] = 1;
      
      nodes++;
      r = min(packItem(index + 1), r);
      has[i][index] = false;
      residual[i] += items[index];
      visited[index] = 0;
      
      if (residual[i] == capacity)
      {
        break;
      }
    }
  }
  
  return r;
}

int main()
{
  start = clock();
  scanf("%d", &capacity);

  int n;
  scanf("%d", &n);
  has.resize(n);
  residual.resize(n, capacity);
  visited.resize(n, 0);
  
  for (int i = 0; i < n; i++)
  {
    int w;
    scanf("%d", &w);
    items.push_back(w);
    has[i].resize(n);
  }
  
  sort(items.begin(), items.end());
  reverse(items.begin(), items.end());
  
  bestL2 = L2(capacity, items)();
  cout << "best L2 " << bestL2 << endl;

  cout << "min bins = " << packItem(0) << endl;
  cout << "nr of visited nodes = " << nodes;
end:
  cout << "time = " << clock() - start << endl;

  return 0;
}


