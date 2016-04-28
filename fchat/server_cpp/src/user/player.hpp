#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <network/i_network.hpp>

class Player
{
public:
  Player() : m_is_online(false), m_network_id(0) {}
  ~Player(){}

  void SetNetID(face2wind::NetworkID net_id) { m_network_id = net_id; }
  void Disconnect();
  int GetId() { return m_network_id; }  
  
protected:
  bool m_is_online;
  
  face2wind::NetworkID m_network_id;
};


#endif
