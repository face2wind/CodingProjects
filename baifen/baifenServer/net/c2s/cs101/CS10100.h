#ifndef _CS10100_H_
#define _CS10100_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 房间列表
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct CS10100 : public face2wind::SocketMessage
{
  /**
   * 页数
   */
  unsigned int pageNum ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt8(pageNum);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    pageNum = data->ReadUnsignedInt8();

  }
};  // class end

#endif  //_CS10100_H_