
#include <iostream>

#include "network/protobuf_network.hpp"
#include "manager/message_manager.hpp"

#include "proto/player.pb.h"
#include "proto/chat.pb.h"

MessageManager::MessageManager()
{
  m_handler_map["protocol.CSChatMessage"] = &MessageManager::OnChatMessageHandler;
}

MessageManager::~MessageManager()
{

}

MessageManager *MessageManager::GetInstance()
{
  static MessageManager instance;
  return &instance;
}

void MessageManager::HandleMessage(NetworkID network_id, const Message *msg)
{
  std::string msg_name = msg->GetTypeName();
  if (m_handler_map.find(msg_name) != m_handler_map.end())
    (this->*m_handler_map[msg_name])(network_id, msg);
}

void MessageManager::OnChatMessageHandler(NetworkID network_id, const Message *msg)
{
  const protocol::CSChatMessage *chat_msg = (const protocol::CSChatMessage *)msg;
  protocol::SCChatMessage sc_msg;
  sc_msg.set_player_id(network_id);
  sc_msg.set_chat_message(chat_msg->chat_message());
  face2wind::ProtobuffNetwork::GetInstance()->SendProtobuffMsg(chat_msg->player_id(), (google::protobuf::Message*)&sc_msg);
}



