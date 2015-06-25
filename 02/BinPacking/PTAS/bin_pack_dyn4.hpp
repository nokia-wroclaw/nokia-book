#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
#include <tuple>
#include <functional>

using namespace std;
namespace dyn4
{

struct X
{
  int a, b, c, d;
};


class dyn4
{
  vector<double> S;
  vector<int> x;
  vector<tuple<int, int, int, int>> CONF;

  vector<double> bins;

  void prepare_conf();
  double capacity;
public:

  dyn4(vector<double> S, vector<int> x, double capacity);

  void calcPath(X cur, X pop);

  vector<double> dp();
};

}


