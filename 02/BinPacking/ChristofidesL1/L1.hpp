#include <vector>
using namespace std;

class L1
{
  vector<int> items;
  int C;
public:
  L1(int C, vector<int> items);
  int operator()();
};

