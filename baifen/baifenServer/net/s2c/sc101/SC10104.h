#ifndef _SC10104_H_
#define _SC10104_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>
#include <customData/RoomInfo.h>


/**
 * 房间信息更新
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10104 : public face2wind::SocketMessage
{
  /**
   * 房间信息
   */
  RoomInfo roomInfo ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->ReadFromByteArray(roomInfo.PackMsg());
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    roomInfo.UnpackMsg(data);

  }
};  // class end

#endif  //_SC10104_H_