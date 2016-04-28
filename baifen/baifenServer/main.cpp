#include <signal.h>
#include <iostream>

#include <networkManager.h>
#include <loginmodule.h>
#include <gatewayModule.h>
#include <baifenmodule.h>
#include <gamemodule.h>

void onSignalHandler(int num)
{
  std::cout<<"receive end signal, close Net!"<<std::endl;
  face2wind::NetworkManager *networkMgr = face2wind::NetworkManager::GetInstance();
  networkMgr->CloseAll();
  exit(1);
}

int main(int argc, char **argv)
{
  signal(SIGINT, onSignalHandler);
  face2wind::NetworkManager *networkMgr = face2wind::NetworkManager::GetInstance();
  networkMgr->AddHandler(new GatewayModule());
  networkMgr->AddHandler(new BaifenModule());
  networkMgr->AddHandler(new GameModule());
  networkMgr->AddHandler(new LoginModule());
  networkMgr->ServerRun();
  return 0;
}
