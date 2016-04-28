#include <networkManager.h>
#include <commandMap.h>
#include <iostream>

namespace face2wind{
  
  NetworkManager *NetworkManager::GetInstance() {
    static NetworkManager m;
    return &m;
  }

  void _ServerRunThread(FSocket *_socket)
  {
    
  }

  void NetworkManager::ServerRun()
  {
    std::cout<<"Server now runnig..."<<std::endl;
    _socket->ServerRun();
  }

  void NetworkManager::ConnectTo(const std::string &ip,const int port)
  {
    _socket->SetPort(port);
    _socket->ClientConnect(ip);
  }

  void NetworkManager::CloseSocket(int socketFD)
  {
    _socket->Disconnect(socketFD);
  }
   
  void NetworkManager::CloseAll()
  {
    _socket->ClientStop();
    _socket->ServerStop();
  }

  void NetworkManager::SendMessage(int cmd,
				   SocketMessage *msg,
				   int socketFD)
  {
    std::cout<<"send message >>> "<<cmd<<std::endl;
    _socket->SendMessage(cmd,msg,socketFD);
  }
    
  void NetworkManager::HandleMsgBytes(int socketFD,ByteArray *msgData)
  {
    short cmd = msgData->ReadUnsignedInt16();
    SocketMessage *msg = CommandMap::GetInstance().GetCSMsgObject(cmd);
    /*   allow empty socket
    if(NULL == msg){
      std::cout<<"message command Map get NULL object with CMD = "
	       <<cmd<<std::endl;
      return;
    }
    */
    //std::cout<<"raw bytes = \n";
    //msgData->ShowAllBytes();
    std::cout<<"receive CS"<<cmd<<", Size "<<msgData->BytesAvailable()<<" bytes  <<<<<<<<<<"<<std::endl;
    if(NULL != msg)
      msg->UnpackMsg(msgData);
    for(std::vector<INetworkHandler*>::iterator it = _handlers.begin();
	it != _handlers.end();it++)
      (*it)->OnReceiveMessage(cmd,msg,socketFD);
  }
  
  void NetworkManager::HandleConnected(const std::string &ip,
				       const int &port,
				       const int &socketFD)
  {
    for(std::vector<INetworkHandler*>::iterator it = _handlers.begin();
	it != _handlers.end();it++)
      (*it)->OnConnected(ip,port,socketFD);
  }

  void NetworkManager::HandleAccepted(const int &socketFD)
  {
    for(std::vector<INetworkHandler*>::iterator it = _handlers.begin();
	it != _handlers.end();it++)
      (*it)->OnAccepted(socketFD);
  }

  void NetworkManager::HandleDisconnect(const int &socketFD)
  {
    for(std::vector<INetworkHandler*>::iterator it = _handlers.begin();
	it != _handlers.end();it++)
      (*it)->OnDisconnected(socketFD);
  }

}
