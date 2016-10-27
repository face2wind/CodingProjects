#include <iostream>
#include <elegance.hpp>

#include "serialize/login_proto.hpp"

using namespace std;
using namespace face2wind;

namespace serialize_demo {

bool has_connect = false;
NetworkID client_net_id = 0;

class ClientNetHandler : public ISerializeNetworkHandler
{
 public:
  virtual void OnListenFail(Port port)
  {}
  virtual void OnAccept(IPAddr ip, Port port, Port local_port, NetworkID net_id)
  {}
  virtual void OnConnect(IPAddr ip, Port port, Port local_port, bool success, NetworkID net_id)
  {
    if (success)
    {
      has_connect = true;
      client_net_id = net_id;
      cout<<"connected : "<<ip<<":"<<port<<endl;
    }
  }

  virtual void OnRecv(NetworkID net_id, const SerializeBase *data)
  {

  }
  
  virtual void OnDisconnect(NetworkID net_id)
  {}
};


class ServerNetHandler : public ISerializeNetworkHandler
{
 public:
  virtual void OnListenFail(Port port)
  {}
  virtual void OnAccept(IPAddr ip, Port port, Port local_port, NetworkID net_id)
  {}
  virtual void OnConnect(IPAddr ip, Port port, Port local_port, bool success, NetworkID net_id)
  {}

  virtual void OnRecv(NetworkID net_id, const SerializeBase *data)
  {
    Protocol::CSRegisterAccount *msg = (Protocol::CSRegisterAccount *)data;
    cout<<"receive name = "<<msg->name<<", password = "<<msg->passwd<<endl;
  }
  
  virtual void OnDisconnect(NetworkID net_id)
  {}
};

class ServerTask : public IThreadTask
{
 public:
  void Run()
  {
    ServerNetHandler handler;
    SerializeNetworkManager mgr;
    mgr.RegistSerializeHandler(&handler);
    mgr.SyncListen(9999);

    mgr.WaitAllThread();
  }
};

}

using namespace serialize_demo;

int main()
{
  Thread t;
  t.Run(new ServerTask());

  Timer::Sleep(1000);

  ClientNetHandler handler;
  SerializeNetworkManager mgr;
  mgr.RegistSerializeHandler(&handler);
  mgr.SyncConnect("127.0.0.1", 9999);
  
  while (!has_connect)
    Timer::Sleep(1000);

  static Protocol::CSRegisterAccount msg;
  msg.name = "face2wind";
  msg.passwd = "mypass";
  mgr.SendSerialize(client_net_id, msg);
  /*
  for (int i = 0; i < INT_MAX; ++i)
  {
    msg.name = "face2wind";
    msg.passwd = "mypass";
    mgr.Send(client_net_id, msg);
    Timer::Sleep(10);
  }
  */
  mgr.WaitAllThread();

  t.Join();
  cout<<"end ......"<<endl;
  
  return 0;
}
