#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <byteArray.h>
#include <fSocket.h>
#include <string>
#include <vector>

namespace face2wind {

  class INetworkHandler;
  class NetworkManager
  {
  private:
    FSocket *_socket;
    
  public:
    NetworkManager(){
      _socket = new FSocket(this);
    }
    ~NetworkManager(){
      delete _socket;
    }

    friend class FSocket;

    static NetworkManager *GetInstance();
    
    void AddHandler(INetworkHandler *handler){
      if(NULL == handler)
	return;
      _handlers.push_back(handler);
    }

    void ServerRun();
    void ConnectTo(const std::string &ip,const int port = 8888);
    void SendMessage(int cmd,
		     SocketMessage *msg,
		     int socketFD = 0);
    void CloseSocket(int);
    void CloseAll();
    
  protected:
    void HandleMsgBytes(int socketFD,ByteArray *msgData);
    void HandleConnected(const std::string &ip,
			 const int &port,
			 const int &socketFD);
    void HandleAccepted(const int &socketFD);
    void HandleDisconnect(const int &socketFD);

  private:
    std::vector<INetworkHandler*> _handlers;
  };

  class INetworkHandler
  {
  protected:
    NetworkManager *networkMgr;
  public:
    INetworkHandler(){
      networkMgr = NetworkManager::GetInstance();
    }
    virtual ~INetworkHandler(){}
    virtual int OnReceiveMessage(unsigned int cmd,
				 SocketMessage *msg,
				 const unsigned int &socketFD) = 0;
    virtual int OnConnected(const std::string &ip,
			    const unsigned int &port,
			    const unsigned int &socketFD) = 0;
    virtual int OnAccepted(const unsigned int &socketFD) = 0;
    virtual int OnDisconnected(const unsigned int &socketFD) = 0;
  };

}

#endif //NETWORK_MANAGER_H
