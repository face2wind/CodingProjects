#include "client_module.hpp"
#include "module/module_driver.hpp"
#include "manager/message_manager.hpp"

#include "limits.h"

#include <iostream>

int ClientModule::Init()
{
  m_network = Network::GetInstance();
  m_proto_network = ProtobuffNetwork::GetInstance();
  return 0;
}

int ClientModule::Start()
{
  return 0;
}

int ClientModule::Update()
{
  static int step(0);
  
  ++step;
  step %= INT_MAX;
  
  return 0;
}

int ClientModule::Stop()
{
  return 0;
}

int ClientModule::Release()
{
  return 0;
}

void ClientModule::OnConnect(bool is_success, NetworkID network_id, Port local_port, IPAddr remote_ip_addr, Port remote_port)
{
  if (false == is_success)
  {
    m_network->Stop();
    face2wind::ModuleDriver::GetInstance()->Exist();
    std::cout<<"connect fail on "<<remote_ip_addr<<":"<<remote_port<<std::endl;
    return;
  }

  if (remote_ip_addr == m_server_ip && remote_port == m_server_port)
  {
    m_server_network_id = network_id;
    MessageManager::GetInstance()->SetServerNetworkID(m_server_network_id);
  }
}

void ClientModule::OnAccept(bool is_success, NetworkID network_id, Port listen_port, IPAddr remote_ip_addr, Port remote_port)
{
  if (false == is_success)
  {
    m_network->Stop();
    face2wind::ModuleDriver::GetInstance()->Exist();
    std::cout<<"connect fail on "<<remote_ip_addr<<":"<<remote_port<<std::endl;
    return;
  }
  
}

void ClientModule::OnRecv(NetworkID network_id, const Message *msg)
{
  MessageManager::GetInstance()->HandleMessage(network_id, msg);
}

void ClientModule::OnDisconnect(NetworkID network_id)
{
  std::cout<<"on socket disconnect"<<std::endl;
  m_network->Stop();
  face2wind::ModuleDriver::GetInstance()->Exist();
 }

void ClientModule::SetServer(IPAddr server_ip, Port server_port)
{
  m_server_ip = server_ip;
  m_server_port = server_port;
}

