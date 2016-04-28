#ifndef F_SOCKET_H
#define F_SOCKET_H

#include <byteArray.h>
#include <socketMessage.h>
#include <map>

namespace face2wind {
  
  class NetworkManager;

  struct _SocketData
  {
    int port;
    int socketFD;
    int bytesBodyLen; // current reading data len (head or dataBody)
    ByteArray *cache;

    _SocketData(int _port, int _fd):
      port(_port),
      socketFD(_fd),
      bytesBodyLen(0)
      {cache = new ByteArray();}
    ~_SocketData(){delete cache;}
  };

  class FSocket
  {
  public:
    FSocket(NetworkManager *handler)
      :_port(8888),
       _maxConcurrencyRequire(10),
       _serverSocketFD(0),
       _serverEpollFD(0),
      //_clientSocketFD(0),
      _clientEpollFD(0),
      _networkMgr(handler)
    {}
    virtual ~FSocket()
    {}

    void SetPort(int port){_port = port;}
    int GetPort(){return _port;}

    int ServerRun();
    int ServerStop();

    int ClientConnect(const std::string &serverIP);
    int ClientStop();

    int SendMessage(int cmd, SocketMessage *msg,int socketFD = 0);
    
    int Disconnect(int socketFD);

  private:
    void ReadSocket(int);
    void ReadRealByteArray(_SocketData *sd);
    //    void HandleMsgBytes(ByteArray *msgData);
    void Setnonblocking(int sock, bool nonblock);

  protected:
    int _port;
    int _maxConcurrencyRequire;
    int _serverSocketFD;
    int _serverEpollFD;
    //int _clientSocketFD;
    int _clientEpollFD;
    NetworkManager *_networkMgr;
    std::map<int,_SocketData*> _serverSocketCache;
    std::map<int,_SocketData*> _clientSocketCache;
  };

}// namespace end

#endif //F_SOCKET_H
