#ifndef _CS10101_H_
#define _CS10101_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 加入房间
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct CS10101 : public face2wind::SocketMessage
{
  /**
   * 房间号
   */
  unsigned int roomID ;

  /**
   * 座位ID（0-3）
   */
  unsigned int sitID ;

  /**
   * 房间密码
   */
  std::string passwd ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt32(roomID);
    by->WriteUnsignedInt8(sitID);
    by->WriteString(passwd);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    roomID = data->ReadUnsignedInt32();
    sitID = data->ReadUnsignedInt8();
    passwd = data->ReadString();

  }
};  // class end

#endif  //_CS10101_H_