#ifndef _SC10103_H_
#define _SC10103_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 踢出、自己退出房间
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10103 : public face2wind::SocketMessage
{
  /**
   * 错误码
   */
  unsigned int result ;

  /**
   * 座位ID（0-3）
   */
  unsigned int sitID ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt16(result);
    by->WriteUnsignedInt8(sitID);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    result = data->ReadUnsignedInt16();
    sitID = data->ReadUnsignedInt8();

  }
};  // class end

#endif  //_SC10103_H_