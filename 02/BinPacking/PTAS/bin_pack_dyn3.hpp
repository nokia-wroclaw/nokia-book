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
  vector<double> S;
  vector<int> x;
  vector<tuple<int, int, int>> CONF;

  vector<double> bins;
  
  void prepare_conf();

public:

  dyn3(vector<double> S, vector<int> x, int capacity);

  void calcPath(X cur, X pop);

  vector<double> dp();
};


}
