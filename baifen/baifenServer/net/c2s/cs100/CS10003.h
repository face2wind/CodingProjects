#ifndef _CS10003_H_
#define _CS10003_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 登录协议
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct CS10003 : public face2wind::SocketMessage
{
  /**
   * 登录名
   */
  std::string loginName ;

  /**
   * 登录密码
   */
  std::string password ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteString(loginName);
    by->WriteString(password);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    loginName = data->ReadString();
    password = data->ReadString();

  }
};  // class end

#endif  //_CS10003_H_