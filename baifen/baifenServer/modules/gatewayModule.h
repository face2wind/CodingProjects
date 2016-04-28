#ifndef GATEWAY_MODULE_H
#define GATEWAY_MODULE_H

#include <networkManager.h>
#include <socketMessage.h>

class GatewayModule : public face2wind::INetworkHandler
{
 public:
  GatewayModule(){}
  ~GatewayModule(){}
  int OnReceiveMessage(unsigned int cmd,
		       face2wind::SocketMessage *msg,
		       const unsigned int &socketFD);
  int OnConnected(const std::string &ip,
		  const unsigned int &port,
		  const unsigned int &socketFD);
  int OnAccepted(const unsigned int &socketFD);
  int OnDisconnected(const unsigned int &socketFD);
};

#endif //GATEWAY_MODULE_H
