#include <iostream>
#include <elegance.hpp>

using namespace std;
using namespace face2wind;

struct AA
{
  char data[128];
};

int main()
{
	MemoryPoolManager mp_mgr;

	long long pre_time = Timer::GetNowTimeMS();
  for (int i = 0; i < 100000; ++ i)
  {
	  //AA *ptr = new AA();
	  //delete ptr;
	  AA *ptr = 
  }
  cout << "loop time : " << Timer::GetNowTimeMS() - pre_time << " ms" << endl;

  return 0;
}
