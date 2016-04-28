#ifndef __MESSAGE_MANAGER_HPP__
#define __MESSAGE_MANAGER_HPP__

#include <map>
#include <string>

#include "network/network.hpp"
#include "google/protobuf/message.h"

using google::protobuf::Message;
using face2wind::NetworkID;

class MessageManager
{
public:
  MessageManager();
  ~MessageManager();
  
  static MessageManager *GetInstance();

  void HandleMessage(NetworkID network_id, const Message *msg);
  void SetServerNetworkID(NetworkID network_id) { m_server_network_id = network_id; }
  NetworkID GetServerNetworkID() { return m_server_network_id; }
  
 protected:
  void OnlinePlayerListHandler(NetworkID network_id, const Message *msg);
  void PlayerInfoHandler(NetworkID network_id, const Message *msg);
  void PlayerMessageHandler(NetworkID network_id, const Message *msg);
  
 protected:
  typedef void (MessageManager::* Handler)(NetworkID network_id, const Message *msg);
  std::map<std::string, Handler> m_handler_map;
  NetworkID m_server_network_id;
};

#endif
