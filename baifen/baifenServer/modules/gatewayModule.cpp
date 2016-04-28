#include <gatewayModule.h>
#include <socketMessage.h>
#include <iostream>

#include <loginmodule.h>

//#include <protocol/TestProto.h>

int GatewayModule::OnReceiveMessage(unsigned int cmd,
				    face2wind::SocketMessage *msg,
				    const unsigned int &socketFD)
{
  /*
  std::cout<<"receive socket message -- "<<cmd<<std::endl;
  if(100 == cmd/100) // 100 , login messages
    LoginModule::GetInstance()->OnReceiveMessage(cmd,msg,socketFD);
  else
    std::cout<<"hasn't define socket number of "<<cmd/100<<std::endl;
  */
  return 0;
}

int GatewayModule::OnConnected(const std::string &ip,
			       const unsigned int &port,
			       const unsigned int &socketFD)
{
  //std::cout<<"on connected to server  -- gateway!"<<std::endl;
  return 0;
}

int GatewayModule::OnAccepted(const unsigned int &socketFD)
{
  //std::cout<<"some one connect -- gateway!"<<std::endl;
  
  //face2wind::NetworkManager *networkMgr =
  //face2wind::NetworkManager::GetInstance();
  //networkMgr->SendMessage(100,t,5);

  return 0;
}

int GatewayModule::OnDisconnected(const unsigned int &socketFD)
{
  
  return 0;
}
