#include "manager/player_list_manager.hpp"
#include "manager/cli_view_manager.hpp"

#include "proto/player.pb.h"
#include "proto/chat.pb.h"

#include <iostream>

PlayerListManager::PlayerListManager() : m_role_id(0)
{
  
}
   
PlayerListManager::~PlayerListManager()
{
  
}
   
PlayerListManager * PlayerListManager::GetInstance()
{
  static PlayerListManager instance;
  return &instance;
}

void PlayerListManager::UpdatePlayerList(const protocol::SCOnlinePlayerList *p_list)
{
  int player_num = p_list->player_size();
  for (int index = 0; index < player_num; ++index)
  {
    const protocol::SCPlayerInfo &player = p_list->player(index);
    if (m_role_id != player.id())
    {
      m_player_map[player.id()].id = player.id();
      m_player_map[player.id()].name = player.name();
    }
    //std::cout<<"id = "<<player.id()<<", name = "<<player.name()<<std::endl;
  }

  CliViewManager::GetInstance()->OnPlayerListChange();
}

void PlayerListManager::PlayerMessage(const protocol::SCChatMessage *chat_msg)
{
  auto player_it = m_player_map.find(chat_msg->player_id());
  if (player_it != m_player_map.end())
  {
    player_it->second.msg_list.push(chat_msg->chat_message());
    if (player_it->second.msg_list.size() > 6)
      player_it->second.msg_list.pop();
    CliViewManager::GetInstance()->OnReceivePlayerMessage(chat_msg);
  }
}

void PlayerListManager::PrintPlayerList()
{
  std::cout<<"\n======== player list ======="<<std::endl;
  for (auto player_it : m_player_map)
  {
    std::cout<<"id = "<<player_it.second.id<<", name = "<<player_it.second.name<<std::endl;
  }
  std::cout<<"============================\n"<<std::endl;
}

bool PlayerListManager::PlayerExist(int player_id)
{
  if (m_player_map.find(player_id) != m_player_map.end())
    return true;

  return false;
}

void PlayerListManager::SetRoleInfo(const protocol::SCPlayerInfo* p_info)
{
  m_role_id = p_info->id();
}



