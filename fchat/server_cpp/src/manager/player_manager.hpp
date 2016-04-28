#ifndef __PLAYER_MANAGER_HPP__
#define __PLAYER_MANAGER_HPP__

#include <map>
#include <string>
#include <network/i_network.hpp>

class Player;

class PlayerManager
{
public:
  static PlayerManager *GetInstance();
  
  void OnPlayerConnect(face2wind::NetworkID network_id);
  void OnPlayerDisconnect(face2wind::NetworkID network_id);
  
  void Update(int step);
  
protected:
  void SendPlayerList(face2wind::NetworkID network_id = 0);
  
  std::map<face2wind::NetworkID, Player*> m_online_player_map;
  std::map<face2wind::NetworkID, Player*> m_offline_player_map;
};

#endif
