
#include "server_module.hpp"
#include "manager/player_manager.hpp"
#include "manager/message_manager.hpp"

#include "module/module_driver.hpp"
#include "limits.h"


#include <iostream>

int ServerModule::Init()
{
  m_network = Network::GetInstance();
  m_proto_network = ProtobuffNetwork::GetInstance();
  return 0;
}

int ServerModule::Start()
{
  return 0;
}

int ServerModule::Update()
{
  static int step(0);
  
  PlayerManager::GetInstance()->Update(step);
  
  ++step;
  step %= INT_MAX;
  
  return 0;
}

int ServerModule::Stop()
{
  return 0;
}

int ServerModule::Release()
{
  return 0;
}

void ServerModule::OnConnect(bool is_success, NetworkID network_id, Port local_port, IPAddr remote_ip_addr, Port remote_port)
{

}

void ServerModule::OnAccept(bool is_success, NetworkID network_id, Port listen_port, IPAddr remote_ip_addr, Port remote_port)
{
  if (false == is_success)
  {
    m_network->Stop();
    face2wind::ModuleDriver::GetInstance()->Exist();
    return;
  }
  
  std::cout<<"receive connect "<<remote_ip_addr<<":"<<remote_port<<std::endl;
  PlayerManager::GetInstance()->OnPlayerConnect(network_id);
}

void ServerModule::OnRecv(NetworkID network_id, const Message *msg)
{
  MessageManager::GetInstance()->HandleMessage(network_id, msg);
}

void ServerModule::OnDisconnect(NetworkID network_id)
{
  PlayerManager::GetInstance()->OnPlayerDisconnect(network_id);
}
