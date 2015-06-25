#include <iostream>
using namespace std;

void generateForFF(const int N, const int x, const int y, const int z)
{
  cout << 128 * 7 * 3 << endl;
  cout << 3 << endl;
  cout << x << " " << y << " " << z << endl;
  cout << N << " " << N << " " << N << endl;
}

int main()
{
  const int N = 2500;
  const int x = 128 * 3 + 7 * 3;
  const int y = 128 * 7 + 7 * 3;
  const int z = 7 * 3 + 64 * 7 * 3;
  generateForFF(N, x, y, z);
  
  return 0;
  
}

