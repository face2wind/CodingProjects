#include "network/protobuf_network.hpp"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <sstream>

#include "manager/cli_view_manager.hpp"
#include "manager/player_list_manager.hpp"
#include "manager/message_manager.hpp"

#include "proto/chat.pb.h"

#include <iostream>
using namespace std;

CliViewManager::CliViewManager() : m_cur_state(CliViewState::MAIN), m_cur_chat_player_id(0)
{

}

CliViewManager::~CliViewManager()
{

}

void CliViewManager::AsyncRun()
{
  boost::thread thread_(boost::bind(&CliViewManager::InputThread, this));
  thread_.detach();
}

CliViewManager * CliViewManager::GetInstance()
{
  static CliViewManager instance;
  return &instance;
}

void CliViewManager::InputThread()
{
  while(true)
  {
    //cout<<">>> ";
    std::string input_str;
    getline(cin, input_str);
    cout<<endl;
    this->AnalysInput(input_str);
    //cin >> input_str;
    //cout << "\b face2wind :\n    "<<input_str << endl<<endl;;
  }
}

void CliViewManager::OnPlayerListChange()
{
  if (m_cur_state == CliViewState::MAIN)
    PlayerListManager::GetInstance()->PrintPlayerList();
}

void CliViewManager::OnReceivePlayerMessage(const protocol::SCChatMessage *chat_msg)
{
  if (m_cur_state == CliViewState::CHATING && m_cur_chat_player_id == chat_msg->player_id())
    std::cout<<chat_msg->player_id()<<" said :\n  "<<chat_msg->chat_message()<<"\n"<<std::endl;
}

void CliViewManager::AnalysInput(const std::string &input_str)
{
  std::stringstream ss;
  ss << input_str;
  
  std::string type;
  ss >> type;
  if (type.size() > 0 && type[0] == '#')
  {
    if (type == "#playerList")
    {
      PlayerListManager::GetInstance()->PrintPlayerList();
    }
    else if (type == "#chat")
    {
      ss >> m_cur_chat_player_id;
      if (PlayerListManager::GetInstance()->PlayerExist(m_cur_chat_player_id))
      {
        m_cur_state = CliViewState::CHATING;
        std::cout<<"now chat to "<<m_cur_chat_player_id<<"\n"<<std::endl;
      }
      else
      {
        std::cout<<"no this user  "<<m_cur_chat_player_id<<std::endl;
      }
    }
    else if (type == "#main")
    {
      m_cur_state = CliViewState::MAIN;
    }
    else if (type == "#quit")
    {
      
    }
    else
    {
      std::cout<<"unknow command : "<<type<<"\n"<<std::endl;
    }
  }
  else
  {
    switch(m_cur_state)
    {
      case CliViewState::MAIN:
        std::cout<<"current in main view, ignore command....\n"<<std::endl;
        break;

      case CliViewState::CHATING:
        {
          //std::cout<<"send this chat message to "<<m_cur_chat_player_id<<"\n"<<std::endl;
          protocol::CSChatMessage chat_msg;
          chat_msg.set_player_id(m_cur_chat_player_id);
          chat_msg.set_chat_message(input_str);
          face2wind::ProtobuffNetwork::GetInstance()->SendProtobuffMsg(MessageManager::GetInstance()->GetServerNetworkID(), (google::protobuf::Message*)&chat_msg);
        }
        break;
    }
  }
}

