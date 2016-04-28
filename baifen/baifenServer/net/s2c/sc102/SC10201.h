#ifndef _SC10201_H_
#define _SC10201_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 请求开始游戏
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10201 : public face2wind::SocketMessage
{
  /**
   * 错误码
   */
  unsigned int result ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt16(result);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    result = data->ReadUnsignedInt16();

  }
};  // class end

#endif  //_SC10201_H_