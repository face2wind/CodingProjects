#ifndef _SC10201_pokerList_H_
#define _SC10201_pokerList_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 发牌
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10201_pokerList : public face2wind::SocketMessage
{
  /**
   * 错误码
   */
  long result ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteShort(result);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    result = data->ReadShort();

  }
};  // class end

#endif  //_SC10201_pokerList_H_