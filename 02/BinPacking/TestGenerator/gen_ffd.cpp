#include <iostream>
using namespace std;

void generateForFFD(const int a, const int b, const int c, const int d, const int k)
{
  const int N1 = 6 * k + 4;
  const int N2 = 6 * k + 4;
  const int N3 = 6 * k + 4 + 2 * (3 * k + 2);
  const int N4 = (3 * k + 2) * 2;
  
  cout << 512 << endl;
  cout << N1 + N2 + N3 + N4 << endl;
  for (int i = 0; i < N1; i++)
  cout << a << endl;
  for (int i = 0; i < N2; i++)
  cout << b << endl;
  for (int i = 0; i < N3; i++)
  cout << c << endl;
  for (int i = 0; i < N4; i++)
  cout << d << endl;
}

int main()
{
  int a = 1 + 256;//1/2
  int b = 1 + 128;//1/4 + e
  int c = 128 - 2;
  int d = 128 + 2;
  
  const int k = 1000;
  generateForFFD(a, b, c, d, k);
  
  return 0;
  
}

