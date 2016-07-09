#include <iostream>
#include <elegance.hpp>

using namespace std;
using namespace face2wind;

struct AAA
{
	char data[128];
};

int main()
{
	MemoryPool pool(sizeof(AAA));
	int LOOP_TIMES = 1000000;

	long long pre_time = Timer::GetNowTimeMS();
  for (int i = 0; i < LOOP_TIMES; ++ i)
  {
	  AAA *ptr = new AAA();
	  delete ptr;
  }
  cout << "new delete loop time : " << Timer::GetNowTimeMS() - pre_time << " ms" << endl;

  pre_time = Timer::GetNowTimeMS();
  for (int i = 0; i < LOOP_TIMES; ++i)
  {
	  AAA *ptr = (AAA *)pool.Alloc();
	  pool.Free(ptr);
  }
  cout << "mempool 222 loop time : " << Timer::GetNowTimeMS() - pre_time << " ms" << endl;

  return 0;
}
