#ifndef __CLI_VIEW_MANAGER_H__
#define __CLI_VIEW_MANAGER_H__

#include <string>

namespace protocol
{
class SCChatMessage;
}

enum class CliViewState
{
  MAIN = 0,
  CHATING
};

class CliViewManager
{
 public:
  static CliViewManager * GetInstance();

  void AsyncRun();

  void InputThread();

  void OnPlayerListChange();
  void OnReceivePlayerMessage(const protocol::SCChatMessage *chat_msg);

 protected:
  CliViewManager();
  ~CliViewManager();

  void AnalysInput(const std::string &input_str);
  
 private:
  CliViewState m_cur_state;
  int m_cur_chat_player_id;

};

#endif
