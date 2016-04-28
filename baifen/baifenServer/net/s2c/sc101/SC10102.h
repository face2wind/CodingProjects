#ifndef _SC10102_H_
#define _SC10102_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 建房间
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10102 : public face2wind::SocketMessage
{
  /**
   * 错误码
   */
  unsigned int result ;

  /**
   * 房间号
   */
  unsigned int roomID ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt16(result);
    by->WriteUnsignedInt32(roomID);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    result = data->ReadUnsignedInt16();
    roomID = data->ReadUnsignedInt32();

  }
};  // class end

#endif  //_SC10102_H_