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
	int LOOP_TIMES = 1000000;

	long long pre_time = Timer::GetNowTimeMS();
  for (int i = 0; i < LOOP_TIMES; ++ i)
  {
	  AA *ptr = new AA();
	  delete ptr;
  }
  cout << "new delete loop time : " << Timer::GetNowTimeMS() - pre_time << " ms" << endl;

  pre_time = Timer::GetNowTimeMS();
  for (int i = 0; i < LOOP_TIMES; ++i)
  {
	  AA *ptr = (AA *)mp_mgr.Alloc(sizeof(AA));
	  mp_mgr.Free(ptr);
  }
  cout << "mempool loop time : " << Timer::GetNowTimeMS() - pre_time << " ms" << endl;

  return 0;
}
