#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>


using namespace std;

vector<int> visited;
vector<int> items;

vector<int> residual;

int capacity;
time_t start;
const int INF = 1 << 30;

int UpperBound = INF;
int nodes = 0;

void update_and_visit(int i, int index)
{
  residual[i] -= items[index];
  visited[index] = true;
}

void restore_and_unvisit(int i, int index)
{
  visited[index] = false;
  residual[i] += items[index];
}

int packItem(int index)
{
  if (clock() - start > 15000) return -1;
  int r = INF;
  if (index > items.size())
    return r;
  
  int s = 0;
  for (int i = 0; i < residual.size(); i++)
  {
    s += residual[i] < capacity;
  }
  if (index == items.size())
  {
    UpperBound = min(UpperBound, s);
    return s;
  }
  
  if (UpperBound != INF && s >= UpperBound)
  {
    return s;
  }
  
  bool stop = false;
  for (int i = 0; !stop && i < residual.size(); i++)
  {
    if (!visited[index] && items[index] <= residual[i])
    {
      if (residual[i] == capacity)
      {
        if (s >= UpperBound - 1) continue;
      }
      update_and_visit(i, index);
      nodes++;
      r = min(packItem(index + 1), r);
      restore_and_unvisit(i, index);
      if (residual[i] == capacity)
        stop = true;
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
  visited.resize(n, 0);
  residual.resize(n, capacity);
  int s = 0;
  for (int i = 0; i < n; i++)
  {
    int w;
    scanf("%d", &w);
    items.push_back(w);
    s += w;
  }
  
  sort(items.begin(), items.end(), [](int a, int b) { return a > b;  });

  cout << "min bins = " << packItem(0) << endl;
  cout << "visited nodes = " << nodes << endl;
end:
  cout << "time = " << clock() - start << endl;
   
  return 0;
}


