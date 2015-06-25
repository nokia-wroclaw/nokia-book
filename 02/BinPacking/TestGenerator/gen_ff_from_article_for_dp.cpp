#include <iostream>
using namespace std;

void generateForFF(const int N, const int x, const int y)
{
  cout << 12 << endl;
  cout << 2 << endl;
  cout << x << " " << y << " " << endl;
  cout << N << " " << N << " " << endl;
}

int main()
{
  const int N = 5;
  const int x = 4 + 1;
  const int y = 6 + 1;
  generateForFF(N, x, y);
  
  return 0;
  
}

