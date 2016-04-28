#ifndef __CLIENT_MODULE_HPP__
#define __CLIENT_MODULE_HPP__

#include "module/i_module.hpp"
#include "network/protobuf_network.hpp"
#include "network/network.hpp"

using face2wind::NetworkID;
using face2wind::Port;
using face2wind::IPAddr;
using face2wind::ProtobuffNetwork;
using face2wind::Network;

using google::protobuf::Message;

class ClientModule : public face2wind::IModule, public face2wind::IProtobufNetworkHandler
{
 public:
  virtual int Init();

  virtual int Start();

  virtual int Update();

  virtual int Stop();

  virtual int Release();

  virtual void OnConnect(bool is_success, NetworkID network_id, Port local_port, IPAddr remote_ip_addr, Port remote_port);
  
  virtual void OnAccept(bool is_success, NetworkID network_id, Port listen_port, IPAddr remote_ip_addr, Port remote_port);
  
  virtual void OnRecv(NetworkID network_id, const Message *msg);
  
  virtual void OnDisconnect(NetworkID network_id);

  void SetServer(IPAddr server_ip, Port server_port);
  
 protected:
  ProtobuffNetwork *m_proto_network;
  Network *m_network;

  IPAddr m_server_ip;
  Port m_server_port;

  NetworkID m_server_network_id;
};

#endif
