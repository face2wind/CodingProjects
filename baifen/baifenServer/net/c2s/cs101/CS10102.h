#ifndef _CS10102_H_
#define _CS10102_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 建房间
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct CS10102 : public face2wind::SocketMessage
{
  /**
   * 加锁类型（0无锁，1需要密码，2一律不准进入）
   */
  unsigned int lockType ;

  /**
   * 房间密码
   */
  std::string passwd ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt8(lockType);
    by->WriteString(passwd);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    lockType = data->ReadUnsignedInt8();
    passwd = data->ReadString();

  }
};  // class end

#endif  //_CS10102_H_