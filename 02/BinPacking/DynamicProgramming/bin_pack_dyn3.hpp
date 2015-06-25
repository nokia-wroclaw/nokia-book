#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
#include <tuple>
#include <functional>

using namespace std;

namespace dyn3
{


struct X
{
  int a, b, c;
};



class dyn3
{
  int capacity;
  vector<int> S;
  vector<int> x;
  vector<tuple<int, int, int>> CONF;

  vector<int> bins;
  
  void prepare_conf();

public:

  dyn3(vector<int> S, vector<int> x, int capacity);

  void calcPath(X cur, X pop);

  vector<int> dp();
};


}
