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
  vector<int> S;
  vector<int> x;
  vector<tuple<int, int, int, int>> CONF;

  vector<int> bins;

  void prepare_conf();
  int capacity;
public:

  dyn4(vector<int> S, vector<int> x, int capacity);

  void calcPath(X cur, X pop);

  vector<int> dp();
};

}


