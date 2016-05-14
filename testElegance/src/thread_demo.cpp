#include <iostream>
#include <elegance.hpp>

using namespace std;
using namespace face2wind;


namespace thread_demo {

	// thread, thread pool ....................
	Mutex mu;
	static bool running = true;
	class MyTask : public IThreadTask
	{
	public:
		void Run()
		{
			cout << "Doing My Task jobs ~~" << endl;
			Timer::Sleep(2000);
		}
	};
	class MySignal : public ISignal
	{
	public:
		MySignal() : ISignal(SignalType::INTERRUPT) {}
		~MySignal() {}

		virtual void OnReceive(SignalType type)
		{
			cout << " oh ,you CTRL + C " << endl;
			running = false;
		}
	};
	int testThreadPool()
	{
#ifdef __LINUX__
		cout << "this is linux" << endl;
#endif

#ifdef __WINDOWS__
		cout << "this is windows" << endl;
#endif

		ThreadPool pool;
		pool.Run(8);

		pool.AddTask(new MyTask());

		MySignal si;

		for (int i = 0; i < 8; ++i)
			//while (running)
		{
			pool.AddTask(new MyTask());
			cout << "add task ...." << endl;
			//Timer::Sleep(500);
		}

		int sec = 0;
		while (running)
		{
			cout << "wait sec " << sec++ << endl;
			Timer::Sleep(1000);
		}

		return 0;
	}

}
