#ifndef _SC10003_testArr_H_
#define _SC10003_testArr_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 测试数组
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10003_testArr : public face2wind::SocketMessage
{
  /**
   * 错误码
   */
  long errCode ;

  /**
   * 错误码2
   */
  long errCode2 ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteShort(errCode);
    by->WriteChar(errCode2);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    errCode = data->ReadShort();
    errCode2 = data->ReadChar();

  }
};  // class end

#endif  //_SC10003_testArr_H_