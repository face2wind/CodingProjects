#include <iostream>
#include <elegance.hpp>

using namespace std;
using namespace face2wind;




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
  cout<<"this is linux"<<endl;
#endif

#ifdef __WINDOWS__
  cout<<"this is windows"<<endl;
#endif

  ThreadPool pool;
  pool.Run(8);

  pool.AddTask(new MyTask());

  MySignal si;
  
  for (int i = 0; i < 8; ++ i)
    //while (running)
  {
    pool.AddTask(new MyTask());
    cout<<"add task ...."<<endl;
    //Timer::Sleep(500);
  }

  int sec = 0;
  while (running)
  {
    cout<<"wait sec "<<sec++<<endl;
    Timer::Sleep(1000);
  }

  return 0;
}

SocketAccept accepter1;
class MyServerHandler : public ISocketHandler
{
public:
	virtual void OnAccept(IPAddr ip, Port port)
	{
		cout << "handler OnAccept(" << ip << ", " << port << ")" << endl;
		char str[12] = "I Am Server";
		for (int i = 0; i < 2; ++i)
		{
			accepter1.Write(ip, port, str, 11);
			Timer::Sleep(1000);
		}
        accepter1.Disconnect(ip, port);
	}
	virtual void OnConnect(IPAddr ip, Port port)
	{
		cout << "handler OnConnect(" << ip << ", " << port << ")" << endl;
	}

	virtual void OnRecv(IPAddr ip, Port port, char *data, int length)
	{
		cout << "Server OnRecv(" << ip << ", " << port << ", \"" << data << "\", " << length << ")" << endl;
	}

	virtual void OnDisconnect(IPAddr ip, Port port)
	{
		cout << "Server OnDisonnect(" << ip << ", " << port << ")" << endl;
	}
};

SocketConnect connect1;
class MyClientHandler : public ISocketHandler
{
public:
	virtual void OnAccept(IPAddr ip, Port port)
	{
		cout << "handler OnAccept(" << ip << ", " << port << ")" << endl;
	}
	virtual void OnConnect(IPAddr ip, Port port)
	{
		cout << "handler OnConnect(" << ip << ", " << port << ")" << endl;
		char str[12] = "I am Client";
		for (int i = 0; i < 2; ++ i)
		{
			connect1.Write(str, 11);
			Timer::Sleep(1000);
		}
		connect1.Disconnect();
	}

	virtual void OnRecv(IPAddr ip, Port port, char *data, int length)
	{
		cout << "Client OnRecv(" << ip << ", " << port << ", \"" << data << "\", " << length << ")" << endl;
	}

	virtual void OnDisconnect(IPAddr ip, Port port)
	{
		cout << "Client OnDisonnect(" << ip << ", " << port << ")" << endl;
	}
};
class ServerTask : public IThreadTask
{
 public:
  void Run()
  {
	  accepter1.ResetHandler(new MyServerHandler());
    cout<<"listen result : "<< accepter1.Listen(9999)<<endl;
  }
};

int thread_main()
{
  Thread t;
  t.Run(new ServerTask());

  Timer::Sleep(1000);

  connect1.ResetHandler(new MyClientHandler());
  if (!connect1.Connect("127.0.0.1", 9999))
    cout<<"connect error"<<endl;

  t.Join();
  cout<<"end ......"<<endl;

  return 0;
}

