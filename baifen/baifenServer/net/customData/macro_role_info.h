#ifndef _macro_role_info_H_
#define _macro_role_info_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct macro_role_info : public face2wind::SocketMessage
{
  /**
   * 角色ID
   */
  double id ;

  /**
   * 角色名称
   */
  std::string name ;

  /**
   * 等级
   */
  unsigned long lev ;

  /**
   * vip等级
   */
  unsigned long vip_lev ;

  /**
   * 头像Id
   */
  unsigned long face_id ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteDouble(id);
    by->WriteString(name);
    by->WriteChar(lev);
    by->WriteChar(vip_lev);
    by->WriteChar(face_id);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    id = data->ReadDouble();
    name = data->ReadString();
    lev = data->ReadChar();
    vip_lev = data->ReadChar();
    face_id = data->ReadChar();

  }
};  // class end

#endif  //_macro_role_info_H_