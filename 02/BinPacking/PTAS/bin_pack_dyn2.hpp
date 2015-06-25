#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
#include <tuple>
#include <functional>

using namespace std;

namespace dyn2
{


struct X
{
  int a, b;
};



class dyn2
{
  int capacity;
  vector<double> S;
  vector<int> x;
  vector<tuple<int, int>> CONF;

  vector<double> bins;
  
  void prepare_conf();

public:

  dyn2(vector<double> S, vector<int> x, double capacity);

  void calcPath(X cur, X pop);

  vector<double> dp();
};
}
