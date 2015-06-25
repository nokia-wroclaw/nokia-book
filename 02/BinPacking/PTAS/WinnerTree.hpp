#include <vector>

using namespace std;

class WinnerTree
{
  int nodes;
  int p;
  int r;
  vector<double> tree;
public:
  WinnerTree(int n);
  
  int winner(double item);
  void update(int j, double item);
  int retp();
  
  int minBins();

};

