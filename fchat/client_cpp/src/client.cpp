#include "network/network.hpp"
#include "module/module_driver.hpp"
#include "common/debug_message.hpp"

#include "modules/client_module.hpp"
#include "manager/cli_view_manager.hpp"

using namespace face2wind;

int main(int argc, char **argv)
{
  IPAddr server_ip("127.0.0.1");
  Port server_port(8888);
  
  if (argc >= 3)
  {
    server_ip = std::string(argv[1]);
    server_port = atoi(argv[2]);
  }
  
  //DebugMessage::GetInstance()->SetOnshowType(DEBUG_MESSAGE_TYPE_BASE_NETWORK, true);

  ClientModule *c_module = new ClientModule();
  c_module->SetServer(server_ip, server_port);

  ProtobuffNetwork::GetInstance()->RegistHandler(c_module);
  ModuleDriver::GetInstance()->RegisterModule("my module", c_module);
  
  std::cout<<"connect to server : ["<<server_ip<<":"<<server_port<<"]"<<std::endl;
  Network::GetInstance()->AsyncConnect(server_ip, server_port);
  
  Network::GetInstance()->AsyncRun();
  CliViewManager::GetInstance()->AsyncRun();
  ModuleDriver::GetInstance()->Run();
  
  return 0;
}
