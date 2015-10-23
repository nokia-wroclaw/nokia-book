#include <vector>
using namespace std;

class L1
{
  vector<int> items;
  int capacity;
public:
  L1(int capacity, vector<int> items);
  int operator()();
};

