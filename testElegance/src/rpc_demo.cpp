#include <iostream>
#include <elegance.hpp>

using namespace std;
using namespace face2wind;

namespace rpc_demo {

class ServerRPCHandler : public face2wind::IRpcHandler
{
 public:
  virtual int HandleCall(const char *data, int length, char *return_data, int &return_length)
  {
    std::cout << "HandleCall Receive data : " << data << std::endl;

    std::string msg("IamServer");
    if (msg.size() > (unsigned int)return_length)
    {
      return_length = 0;
      return -1;
    }

    memcpy(return_data, msg.c_str(), msg.size());
    return_length = msg.size();

    return 0;
  }
};

class ClientRPCRequest : public face2wind::IRpcRequest
{
 public:
  virtual void OnCallBack(const char *data, int length)
  {
    std::cout << "client receive : " << data << std::endl;
  }
};

class RPCServerHandler : public IRpcConnectHandler
{
 public:
  RPCServerHandler() {}
  ~RPCServerHandler() {}

  virtual void OnListenFail(Port listen_port)
  {
    std::cout << "Server OnListenFail" << std::endl;
  }

  virtual void OnConnectFail(IPAddr remote_ip_addr, Port remote_port)
  {
    std::cout << "Server OnConnectFail" << std::endl;
  }

  virtual void OnSessionActive(RPCSession *session)
  {
    std::cout << "Server OnSessionActive" << std::endl;
    session->RegisterHandler(new ServerRPCHandler());
  }

  virtual void OnSessionInactive(RPCSession *session)
  {
    std::cout << "Server OnSessionActive" << std::endl;
  }
};

class RPCClientHandler : public IRpcConnectHandler
{
 public:
  virtual void OnListenFail(Port listen_port)
  {
    std::cout << "Client OnListenFail" << std::endl;
  }

  virtual void OnConnectFail(IPAddr remote_ip_addr, Port remote_port)
  {
    std::cout << "Client OnConnectFail" << std::endl;
  }

  virtual void OnSessionActive(RPCSession *session)
  {
    //session->AsyncCall()
    std::cout << "Client OnSessionActive" << std::endl;
    session->AsyncCall(new ClientRPCRequest(), "ILoveYou", 9);
  }

  virtual void OnSessionInactive(RPCSession *session)
  {
    std::cout << "Client OnSessionInactive" << std::endl;
  }
};

class ThreadDemoServerTask : public IThreadTask
{
 public:
  void Run()
  {
    NetworkManager net_mgr;
    RPCServerHandler *server = new RPCServerHandler();
    RPCManager rpc_mgr(server, &net_mgr);
    rpc_mgr.AsyncListen(8989, "keykey");

    net_mgr.WaitAllThread();
  }
};
}

int main()
{
  using namespace rpc_demo;

  DebugMessage::GetInstance().SetOnshowType(DebugMessageType::BASE_NETWORK, true);
  
  Thread t;
  t.Run(new ThreadDemoServerTask());

  Timer::Sleep(1000);

  NetworkManager net_mgr;
  RPCManager rpc_mgr(new RPCClientHandler(), &net_mgr);
  rpc_mgr.AsyncConnect("127.0.0.1", 8989, "keykey");
  

  net_mgr.WaitAllThread();
  t.Join();

  cout<<"end ......"<<endl;
}

