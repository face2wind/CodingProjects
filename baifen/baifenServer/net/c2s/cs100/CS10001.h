#ifndef _CS10001_H_
#define _CS10001_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 帐号查询
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct CS10001 : public face2wind::SocketMessage
{
  /**
   * 登录名
   */
  std::string loginName ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteString(loginName);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    loginName = data->ReadString();

  }
};  // class end

#endif  //_CS10001_H_