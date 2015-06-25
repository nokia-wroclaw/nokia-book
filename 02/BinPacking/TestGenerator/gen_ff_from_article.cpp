#include <iostream>
using namespace std;

void generateForFF(const int N, const int x, const int y)
{
  cout << 12 << endl;
  cout << 2 * N << endl;
  for (int i = 0; i < N; i++)
  cout << x << endl;
  for (int i = 0; i < N; i++)
  cout << y << endl;
}

int main()
{
  const int N = 15;
  const int x = 4 + 1;
  const int y = 6 + 1;
  generateForFF(N, x, y);
  
  return 0;
  
}

