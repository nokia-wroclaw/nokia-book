#include <vector>
using namespace std;

class L2
{
  vector<int> items;
  int capacity;
  int r1 = -1;
  int r2 = -1;
  int r3 = 1000000;
public:
  L2(int capacity, vector<int> items) ;
  int operator()();
  int calcL2(int alfa);
  int calcNext(int alfa);
  
  int jstar = -1;
  int sJ2star = 0;
  void find();
  
private:
  int J1size = 0;
  int J2size = 0;
  int J3size = 0;
  int sJ1 = 0;
  int sJ2 = 0;
  int sJ3 = 0;
};

