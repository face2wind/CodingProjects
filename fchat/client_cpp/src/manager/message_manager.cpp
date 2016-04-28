#include <iostream>
#include "manager/message_manager.hpp"
#include "manager/player_list_manager.hpp"

#include "proto/player.pb.h"

MessageManager::MessageManager()
{
  m_handler_map["protocol.SCOnlinePlayerList"] = &MessageManager::OnlinePlayerListHandler;
  m_handler_map["protocol.SCPlayerInfo"] = &MessageManager::PlayerInfoHandler;
  m_handler_map["protocol.SCChatMessage"] = &MessageManager::PlayerMessageHandler;
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

void MessageManager::OnlinePlayerListHandler(NetworkID network_id, const Message *msg)
{
  const protocol::SCOnlinePlayerList *p_list = (const protocol::SCOnlinePlayerList *)msg;
  PlayerListManager::GetInstance()->UpdatePlayerList(p_list);
}

void MessageManager::PlayerInfoHandler(NetworkID network_id, const Message *msg)
{
  const protocol::SCPlayerInfo *p_info = (const protocol::SCPlayerInfo *)msg;
  PlayerListManager::GetInstance()->SetRoleInfo(p_info);
}

void MessageManager::PlayerMessageHandler(NetworkID network_id, const Message *msg)
{
  const protocol::SCChatMessage *chat_msg = (const protocol::SCChatMessage *)msg;
  PlayerListManager::GetInstance()->PlayerMessage(chat_msg);
}

