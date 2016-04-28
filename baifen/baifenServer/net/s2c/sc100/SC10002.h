#ifndef _SC10002_H_
#define _SC10002_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 创建帐号
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10002 : public face2wind::SocketMessage
{
  /**
   * 错误码
   */
  unsigned int errCode ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt16(errCode);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    errCode = data->ReadUnsignedInt16();

  }
};  // class end

#endif  //_SC10002_H_