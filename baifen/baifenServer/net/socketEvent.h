#ifndef SOCKET_EVENT_HPP
#define SOCKET_EVENT_HPP

#include "eventDispatcher.h"

namespace face2wind {

  struct SocketSendEvent : public ParamEvent
  {
    int socketFD;
    short cmd;
    SocketMessage *msg;;
  };
}

#endif //SOCKET_EVENT_HPP
