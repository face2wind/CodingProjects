#ifndef SOCKET_MESSSAGE_HPP
#define SOCKET_MESSSAGE_HPP

#include "byteArray.h"

namespace face2wind {

  class SocketMessage
  {
  public:
    virtual ByteArray *PackMsg() = 0;
    virtual void UnpackMsg(ByteArray *data) = 0;

    SocketMessage():MsgID(0){}
    virtual ~SocketMessage(){}
  
  public:
    int MsgID;
  };

}

#endif //SOCKET_MESSSAGE_HPP
