#ifndef _SC10001_H_
#define _SC10001_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 帐号查询
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10001 : public face2wind::SocketMessage
{
  /**
   * 帐号是否已存在，1 存在，0 不存在
   */
  unsigned int accountExist ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt8(accountExist);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    accountExist = data->ReadUnsignedInt8();

  }
};  // class end

#endif  //_SC10001_H_