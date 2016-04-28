#include <iostream>
#include <string>
#include "command_analyser.hpp"
#include "server_network_handler.hpp"
#include "client_network_handler.hpp"
#include "common/debug_message.hpp"
#include "cmake_config.h"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    CommandAnalyser::ShowHelp();
    return 1;
  }

  CommandAnalyser analyser(argc, argv);
  if (!analyser.CheckAllParams())
  {
    return 2;
  }

	//DebugMessage::GetInstance()->SetOnshowType(DebugMessageType::BASE_NETWORK);
  if (analyser.CheckCommandState(COMMAND_TYPE_IS_SERVER))
  {
    ServerNetworkHandler *server_handler = new ServerNetworkHandler(&analyser);
    server_handler->Running();
  }
  else if (analyser.CheckCommandState(COMMAND_TYPE_IS_CLIENT))
	{
    ClientNetworkHandler *client_handler = new ClientNetworkHandler(&analyser);
    client_handler->Running();
  }
	else if (analyser.CheckCommandState(COMMAND_TYPE_IS_SHOW_VERSION))
	{
		std::cout << argv[0] << ", version " 
			<< fileTransfer_VERSION_MAJOR << "." 
			<< fileTransfer_VERSION_MINOR << "." 
			<< fileTransfer_VERSION_PATCH << std::endl;
	}
  return 0;
}
