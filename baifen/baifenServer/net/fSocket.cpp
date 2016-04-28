#include <fSocket.h>
#include <byteArray.h>
#include <commandMap.h>
#include <networkManager.h>

#include <iostream>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <arpa/inet.h> // for inet_pton

namespace face2wind {

  void FSocket::Setnonblocking(int sock, bool nonblock)
  {
    int opts;
    opts = fcntl(sock,F_GETFL);
    if(opts<0){
      perror("fcntl(sock,GETFL)");
      return;//exit(1);
    }
    if(nonblock)
      opts = opts|O_NONBLOCK;
    else
      opts = opts&~O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0) {
      perror("fcntl(sock,SETFL,opts)");
      return;//exit(1);
    }
  }

  void FSocket::ReadSocket(int socketFD)
  {
    _SocketData *sd = NULL;
    if(0 != _serverSocketCache.count(socketFD))
      sd = _serverSocketCache[socketFD];
    else if(0 != _clientSocketCache.count(socketFD))
      sd = _clientSocketCache[socketFD];
    else
      return;
    if(1 == sd->cache->ReadAllDataFromSocket(socketFD)){ // disconnect
      _networkMgr->HandleDisconnect(socketFD);
    }
    ReadRealByteArray(sd);
  }

  void FSocket::ReadRealByteArray( _SocketData *sd)
  {
    if(0 == sd->bytesBodyLen){ // hasn't read head
      if(2 > sd->cache->BytesAvailable()){ // not enough bytes for head
	//std::cout<<"not enough bytes for head"<<std::endl;
	return;
      }
      sd->bytesBodyLen = sd->cache->ReadUnsignedInt16();
    }else{
      if(sd->bytesBodyLen > sd->cache->BytesAvailable()) {
	//std::cout<<"not enough bytes for body"<<std::endl;
	return; // not enough bytes for body
      }
      ByteArray *body = new ByteArray();
      body->ReadFromByteArray(sd->cache,sd->bytesBodyLen);
      _networkMgr->HandleMsgBytes(sd->socketFD, body);
      sd->bytesBodyLen = 0;
    }
    if(0 < sd->cache)
      ReadRealByteArray(sd);
  }
  /*  
  void FSocket::HandleMsgBytes(ByteArray *msgData)
  {
    short cmd = msgData->ReadUnsignedInt16();
    SocketMessage *msg = CommandMap::GetInstance().GetCSMsgObject(cmd);
    if(NULL == msg){
      std::cout<<"message cmd get NULL object"<<std::endl;
      return;
    }
    msg->UnpackMsg(msgData);

    TestProto *p = static_cast<TestProto*>(msg);
    std::cout<<"name = "<<p->name<<", id = "<<p->id<<std::endl;
  }
  */
  int FSocket::ServerRun()
  {
    struct sockaddr_in my_addr;
    struct sockaddr_in remote_addr;
    if ((_serverSocketFD = socket(AF_INET,  SOCK_STREAM,  0)) == -1) {
      perror("socket");
      exit(1);
    }
    int on = 1;
    int ret = setsockopt( _serverSocketFD, 
			  SOL_SOCKET, 
			  SO_REUSEADDR, &on, sizeof(on) );
    ret = ret;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(_port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(my_addr.sin_zero), 8);
    if (bind(_serverSocketFD,  (struct sockaddr *)&my_addr,
	     sizeof(struct sockaddr)) == -1) {
      perror("bind");
      exit(1);
    }
    std::cout<<"server bind ok ,port = "<<_port<<std::endl;
    if (listen(_serverSocketFD,_maxConcurrencyRequire) == -1) {
      perror("listen");
      exit(1);
    }
    std::cout<<"server listen ok"<<std::endl;
    socklen_t sin_size = sizeof(struct sockaddr_in);

#define MAX_EVENTS 1
    struct epoll_event ev, events[MAX_EVENTS];
    int conn_sock, nfds;

    _serverEpollFD = epoll_create(10);
    if (_serverEpollFD == -1) {
      perror("epoll_create");
      exit(EXIT_FAILURE);
    }
    std::cout<<"epoll create OK"<<std::endl;

    ev.events = EPOLLIN;
    ev.data.fd = _serverSocketFD;
    if (epoll_ctl(_serverEpollFD, EPOLL_CTL_ADD, _serverSocketFD, &ev)
	== -1) {
      perror("epoll_ctl: _serverSocketFD");
      exit(EXIT_FAILURE);
    }
    std::cout<<"epoll_ctl ok"<<std::endl;

    while(true) {
      std::cout<<"start epoll_wait............."<<std::endl;
      nfds = epoll_wait(_serverEpollFD, events, MAX_EVENTS, -1);
      if (nfds == -1) {
	perror("epoll_pwait");
	exit(EXIT_FAILURE);
      }
      for (int n = 0; n < nfds; ++n) {
	if (events[n].data.fd == _serverSocketFD) {
	  // build socket connection
	  conn_sock =
	    accept(_serverSocketFD,
		   (struct sockaddr *) &remote_addr, &sin_size);
	  if (conn_sock == -1) {
	    perror("accept");
	    exit(EXIT_FAILURE);
	  }
	  Setnonblocking(conn_sock,true);
	  ev.events = EPOLLIN | EPOLLET;
	  ev.data.fd = conn_sock;
	  if (epoll_ctl(_serverEpollFD, EPOLL_CTL_ADD, conn_sock, &ev)
	      == -1) {
	    perror("epoll_ctl: conn_sock");
	    exit(EXIT_FAILURE);
	  }
	  _serverSocketCache[conn_sock] = new _SocketData(_port,conn_sock);
	  std::cout<<"socket accept success with id = "
		   <<conn_sock<<std::endl;
	  _networkMgr->HandleAccepted(conn_sock);
 	} else if (events[n].events & EPOLLIN) {
	  ReadSocket(events[n].data.fd);
	} else {
	}
      }
    }
  }
  
  int FSocket::ServerStop()
  {
    for(std::map<int,_SocketData*>::iterator it = _serverSocketCache.begin();
	it != _serverSocketCache.end(); ++it)
	close(it->second->socketFD);
    close(_serverEpollFD);
    close(_serverSocketFD);
    return 0;
  }

  int FSocket::ClientConnect(const std::string &serverIP)
  {
    char addr[16];
    int _clientSocketFD;
    struct sockaddr_in serverAddr;
    //char recvline[MAXLINE];//, sendline[MAXLINE];
    strcpy(addr,serverIP.c_str());
    if( (_clientSocketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
      printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
      exit(0);
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port);
    if( inet_pton(AF_INET, addr, &serverAddr.sin_addr) <= 0){
      printf("inet_pton error for %s\n", addr);
      exit(0);
    }
    if( connect(_clientSocketFD, 
		(struct sockaddr*)&serverAddr,
		sizeof(serverAddr)) < 0){
      printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
      exit(0);
    }
    // ===================================================
    
    Setnonblocking(_clientSocketFD,true);
    
#define MAX_EVENTS 1
    struct epoll_event ev, events[MAX_EVENTS];
    int nfds;

    _clientEpollFD = epoll_create(10);
    if (_clientEpollFD == -1) {
      perror("epoll_create");
      exit(EXIT_FAILURE);
    }
    //std::cout<<"epoll_create"<<std::endl;

    ev.events = EPOLLIN;
    ev.data.fd = _clientSocketFD;
    if (epoll_ctl(_clientEpollFD, 
		  EPOLL_CTL_ADD, 
		  _clientSocketFD, &ev) == -1) {
      perror("epoll_ctl: _clientSocketFD");
      exit(EXIT_FAILURE);
    }
    _clientSocketCache[_clientSocketFD]
      = new _SocketData(_port,_clientSocketFD);
    std::cout<<"client connect ok"<<std::endl;
    _networkMgr->HandleConnected(serverIP,_port,_clientSocketFD);

    while(true) {
      //std::cout<<"start epoll_wait.............\n";
      nfds = epoll_wait(_clientEpollFD, events, MAX_EVENTS, -1);
      if (nfds == -1) {
	perror("epoll_pwait");
	exit(EXIT_FAILURE);
      }
      for (int n = 0; n < nfds; ++n) {
	if (events[n].data.fd == _clientSocketFD) {
	  // build socket connection
	} else if (events[n].events & EPOLLIN) {
	  ReadSocket(events[n].data.fd);
	} else {
	}
      }
    }    
    return 0;
  }

  int FSocket::ClientStop()
  {
    for(std::map<int,_SocketData*>::iterator it = _clientSocketCache.begin();
	it != _clientSocketCache.end(); ++it)
	close(it->second->socketFD);
    close(_clientEpollFD);
    //close(_clientSocketFD);
    return 0;
  }

  int FSocket::SendMessage(int cmd, SocketMessage *msg,int socketFD)
  {
    if(0 == _serverSocketCache.count(socketFD) &&
       0 == _clientSocketCache.count(socketFD) ) {
      //std::cout<<"_serverSocketCache.count("<<socketFD<<") = "<<_serverSocketCache.count(socketFD)<<std::endl;
      //std::cout<<"_clientSocketCache.count("<<socketFD<<") = "<<_clientSocketCache.count(socketFD)<<std::endl;
      std::cout<<"this socketFD ("<<socketFD<<") are not connected"<<std::endl;
      return 1;
    }
    face2wind::ByteArray tmp;
    face2wind::ByteArray head;
    tmp.WriteUnsignedInt16(cmd);
    face2wind::ByteArray *msgBy = msg->PackMsg();
    std::cout<<"send SC"<<cmd<<", Size "<<msgBy->BytesAvailable()<<" bytes  >>>>>>>>>>"<<std::endl;
    tmp.ReadFromByteArray(msgBy);
    head.WriteUnsignedInt16(tmp.BytesAvailable());
    head.WriteAllDataToSocket(socketFD);
    tmp.WriteAllDataToSocket(socketFD);
    return 0;
  }
    
  int FSocket::Disconnect(int socketFD)
  {
    close(socketFD);
    return 0;
  }

}
