#include <iostream>
#include <elegance.hpp>

using namespace std;
using namespace face2wind;

Mutex network_demo_mutex;

bool has_connect = false;
NetworkID client_net_id = 0;

class MyServerHandler : public INetworkHandler
{
 public:
  void OnListenFail(Port port)
  {
    network_demo_mutex.Lock();
    cout << "server OnListenFail(" << port << ")" << endl;
    network_demo_mutex.Unlock();
  }

  void OnAccept(IPAddr ip, Port port, NetworkID net_id)
  {
    network_demo_mutex.Lock();
    cout << "server OnAccept(" << ip << ", " << port << ", " << net_id << ")" << endl;
    network_demo_mutex.Unlock();
    /*
      char str[12] = "I Am Server";
      for (int i = 0; i < 2; ++i)
      {
      accepter1.Write(ip, port, str, 11);
      Timer::Sleep(1000);
      }
    */
  }

  void OnConnect(IPAddr ip, Port port, bool success, NetworkID net_id)
  {
    network_demo_mutex.Lock();
    cout << "server OnConnect(" << ip << ", " << port << ")" << endl;
    network_demo_mutex.Unlock();
  }

  void OnRecv(NetworkID net_id, char *data, int length)
  {
    network_demo_mutex.Lock();
    cout << "Server OnRecv(" << net_id << ", \"" << data << "\", " << length << ")" << endl;
    network_demo_mutex.Unlock();
  }

  void OnDisconnect(NetworkID net_id)
  {
    network_demo_mutex.Lock();
    cout << "Server OnDisonnect(" << net_id << ")" << endl;
    network_demo_mutex.Unlock();
  }
};

class MyClientHandler : public INetworkHandler
{
 public:
  void OnListenFail(Port port)
  {
    network_demo_mutex.Lock();
    cout << "client OnListenFail(" << port << ")" << endl;
    network_demo_mutex.Unlock();
  }

  void OnAccept(IPAddr ip, Port port, NetworkID net_id)
  {
    network_demo_mutex.Lock();
    cout << "client OnAccept(" << ip << ", " << port << ", " << net_id << ")" << endl;
    network_demo_mutex.Unlock();
    /*
      char str[12] = "I Am Server";
      for (int i = 0; i < 2; ++i)
      {
      accepter1.Write(ip, port, str, 11);
      Timer::Sleep(1000);
      }
    */
  }

  void OnConnect(IPAddr ip, Port port, bool success, NetworkID net_id)
  {
    network_demo_mutex.Lock();
    cout << "client OnConnect(" << ip << ", " << port << ")" << endl;
    network_demo_mutex.Unlock();

    has_connect = true;
    client_net_id = net_id;
  }

  void OnRecv(NetworkID net_id, char *data, int length)
  {
    network_demo_mutex.Lock();
    cout << "client OnRecv(" << net_id << ", \"" << data << "\", " << length << ")" << endl;
    network_demo_mutex.Unlock();
  }

  void OnDisconnect(NetworkID net_id)
  {
    network_demo_mutex.Lock();
    cout << "client OnDisonnect(" << net_id << ")" << endl;
    network_demo_mutex.Unlock();
  }
};

class ServerTask : public IThreadTask
{
 public:
  void Run()
  {
    MyServerHandler handler;
    NetworkManager mgr;
    mgr.RegistHandler(&handler);
    mgr.SyncListen(9999);
	  
    mgr.WaitAllThread();
  }
};

int main()
{
  Thread t;
  t.Run(new ServerTask());

  Timer::Sleep(1000);

  MyClientHandler handler;
  NetworkManager mgr;
  mgr.RegistHandler(&handler);
  mgr.SyncConnect("127.0.0.1", 9999);
  
  while (!has_connect)
    Timer::Sleep(1000);

  for (int i = 0; i < 10; ++i)
  {
    mgr.Send(client_net_id, "i am client!", 12);
    Timer::Sleep(1000);
  }

  mgr.WaitAllThread();

  t.Join();
  cout<<"end ......"<<endl;
}

