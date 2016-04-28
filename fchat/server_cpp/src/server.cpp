#include "network/network.hpp"
#include "module/module_driver.hpp"
#include "common/debug_message.hpp"

#include "modules/server_module.hpp"

using namespace face2wind;

int main(int argc, char **argv)
{
  Port server_port(8888);
  if (argc >= 2)
    server_port = atoi(argv[1]);
  
  DebugMessage::GetInstance()->SetOnshowType(DEBUG_MESSAGE_TYPE_BASE_NETWORK, true);

  ServerModule *s_module = new ServerModule();

  ProtobuffNetwork::GetInstance()->RegistHandler(s_module);
  ModuleDriver::GetInstance()->RegisterModule("my module", s_module);
  
  std::cout<<"listen on port "<<server_port<<std::endl;
  Network::GetInstance()->AsyncListen(server_port);
  
  Network::GetInstance()->AsyncRun();
  ModuleDriver::GetInstance()->Run();
  
  return 0;
}
