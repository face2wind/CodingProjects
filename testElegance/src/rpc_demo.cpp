#include <iostream>
#include <elegance.hpp>

using namespace std;
using namespace face2wind;

namespace rpc_demo {

	class RPCServerHandler : public IRpcConnectHandler
	{
	public:
		RPCServerHandler() {}
		~RPCServerHandler() {}

		virtual void OnListenFail(Port listen_port)
		{

		}

		virtual void OnConnectFail(IPAddr remote_ip_addr, Port remote_port)
		{

		}

		virtual void OnSessionActive(RPCSession *session)
		{

		}

		virtual void OnSessionInactive(RPCSession *session)
		{

		}
	};

	class RPCClientHandler : public IRpcConnectHandler
	{
	public:
		virtual void OnListenFail(Port listen_port)
		{

		}

		virtual void OnConnectFail(IPAddr remote_ip_addr, Port remote_port)
		{

		}

		virtual void OnSessionActive(RPCSession *session)
		{
			//session->AsyncCall()
		}

		virtual void OnSessionInactive(RPCSession *session)
		{

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
			rpc_mgr.AsyncListen(9999, "keykey");

			net_mgr.WaitAllThread();
		}
	};
}

int main()
{
	using namespace rpc_demo;

  Thread t;
  t.Run(new ThreadDemoServerTask());

  Timer::Sleep(1000);

  NetworkManager net_mgr;
  RPCManager rpc_mgr(new RPCClientHandler(), &net_mgr);
  rpc_mgr.AsyncConnect("127.0.0.1", 9999, "keykey");
  

  net_mgr.WaitAllThread();
  t.Join();

  cout<<"end ......"<<endl;
}

