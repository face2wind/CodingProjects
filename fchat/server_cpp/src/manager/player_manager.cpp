#include "manager/player_manager.hpp"
#include "network/protobuf_network.hpp"
#include "user/player.hpp"

#include "proto/player.pb.h"

PlayerManager *PlayerManager::GetInstance()
{
  static PlayerManager instance;
  return &instance;
}
  
void PlayerManager::OnPlayerConnect(face2wind::NetworkID network_id)
{
  Player *player = NULL;
  auto online_player_it = m_online_player_map.find(network_id);
  if (online_player_it != m_online_player_map.end())
    return;
  
  auto off_player_it = m_offline_player_map.find(network_id);
  if (off_player_it != m_offline_player_map.end())
  {
    player = off_player_it->second;
    m_offline_player_map.erase(off_player_it);
  }
  else
  {
    player = new Player();
  }

  {
    protocol::SCPlayerInfo p_info;
    p_info.set_id(network_id);
    p_info.set_name("player name");
    face2wind::ProtobuffNetwork::GetInstance()->SendProtobuffMsg(network_id, (google::protobuf::Message*)&p_info);
  }
  
  player->SetNetID(network_id);
  m_online_player_map[network_id] = player;
  //this->SendPlayerList(network_id);
  this->SendPlayerList();
  
  return;
}

void PlayerManager::OnPlayerDisconnect(face2wind::NetworkID network_id)
{
  auto online_player_it = m_online_player_map.find(network_id);
  if (online_player_it != m_online_player_map.end())
  {
    m_offline_player_map[network_id] = online_player_it->second;
    m_online_player_map.erase(online_player_it);
  }
  return;
}
  
void PlayerManager::Update(int step)
{
  
}

void PlayerManager::SendPlayerList(face2wind::NetworkID network_id)
{
  protocol::SCPlayerInfo *info = NULL;
  
  protocol::SCOnlinePlayerList online_player_list;
  for (auto it = m_online_player_map.begin(); it != m_online_player_map.end(); ++it)
  {
    info = online_player_list.add_player();
    info->set_id(it->second->GetId());
    info->set_name("xxx");
  }

  if (network_id <= 0) // broadcast
  {
    for (auto it = m_online_player_map.begin(); it != m_online_player_map.end(); ++it)
      face2wind::ProtobuffNetwork::GetInstance()->SendProtobuffMsg(it->second->GetId(), (google::protobuf::Message*)&online_player_list);
  }
  else
  {
    face2wind::ProtobuffNetwork::GetInstance()->SendProtobuffMsg(network_id, (google::protobuf::Message*)&online_player_list);
  }
}
