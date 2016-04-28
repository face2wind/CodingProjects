#ifndef _SC10000_H_
#define _SC10000_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 心跳包
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10000 : public face2wind::SocketMessage
{
  /**
   * 当前时间戳
   */
  unsigned int timeStamp ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt32(timeStamp);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    timeStamp = data->ReadUnsignedInt32();

  }
};  // class end

#endif  //_SC10000_H_