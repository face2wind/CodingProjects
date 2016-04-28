#ifndef _SC10003_H_
#define _SC10003_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 登录协议
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10003 : public face2wind::SocketMessage
{
  /**
   * 错误码
   */
  unsigned int errCode ;

  /**
   * 主角ID
   */
  unsigned int id ;

  /**
   * 主角名
   */
  std::string name ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt16(errCode);
    by->WriteUnsignedInt32(id);
    by->WriteString(name);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    errCode = data->ReadUnsignedInt16();
    id = data->ReadUnsignedInt32();
    name = data->ReadString();

  }
};  // class end

#endif  //_SC10003_H_