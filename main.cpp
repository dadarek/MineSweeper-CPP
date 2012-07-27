#include <iostream>
using namespace std;

class X
{
  public:

  static void hi()
  {
    cout << "hi" << endl;
  }
};

int main()
{
  
  X::hi();

  return 0; 
}

