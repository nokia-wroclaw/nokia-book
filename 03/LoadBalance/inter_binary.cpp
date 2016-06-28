#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;
bool IsOdd (int i) { return ((i%2)==1); }

vector<int> intersect(vector<int> & input1, vector<int> & input2)
{
  vector<int> output;
  
  copy_if(input1.begin(), input1.end(), back_inserter(output), [&](int t) {
    return binary_search(input2.begin(), input2.end(), t);
  });
  
  return output;
}

int main()
{
  vector<int> d;
  vector<int> in1;
  in1 = {4, 1, 9, 6, 7};
 
  vector<int> in2;
  in2 = {1, 5, 7, 8, 9, 20};
    
  d = intersect(in1, in2);
//  diff(in1, in2);
  /*in1.erase(remove_if(in1.begin(), in1.end(), [&](int t) {
    auto it = in2.find(t);
    return it != in2.end();
    
  }), in1.end());*/
  
  //in1.erase(it);

  for (auto i = d.begin(); i != d.end(); i++)
  {
    cout << *i << " ";
  }
  
  


  int myints[] = {1,2,3,4,5,6,7,8,9};            // 1 2 3 4 5 6 7 8 9

  // bounds of range:
  int* pbegin = myints;                          // ^
  int* pend = myints+sizeof(myints)/sizeof(int); // ^                 ^

  pend = std::remove_if (pbegin, pend, IsOdd);   // 2 4 6 8 ? ? ? ? ?
                                                 // ^       ^
  std::cout << "the range contains:";
  for (int* p=pbegin; p!=pend; ++p)
    std::cout << ' ' << *p;
  std::cout << '\n';
  
  return 0;
}

