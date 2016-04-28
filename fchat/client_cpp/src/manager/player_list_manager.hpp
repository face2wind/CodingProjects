#ifndef __PLAYER_LIST_MANAGER_HPP__
#define __PLAYER_LIST_MANAGER_HPP__

#include <map>
#include <stack>
#include <string>

namespace protocol
{
class SCOnlinePlayerList;
class SCPlayerInfo;
class SCChatMessage;
}

struct ChatPlayer
{
  ChatPlayer() : id(0), name("") {}
  
  int id;
  std::string name;
  
  std::stack<std::string> msg_list;
};

class PlayerListManager
{
 public:
  static PlayerListManager * GetInstance();

  void UpdatePlayerList(const protocol::SCOnlinePlayerList *p_list);
  void PlayerMessage(const protocol::SCChatMessage *chat_msg);

  void PrintPlayerList();
  bool PlayerExist(int player_id);
  void SetRoleInfo(const protocol::SCPlayerInfo* p_info);

 protected:
   PlayerListManager();
   ~PlayerListManager();
   
  std::map<int, ChatPlayer> m_player_map;
  int m_role_id;
  
};

#endif
