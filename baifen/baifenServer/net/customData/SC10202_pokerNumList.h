#ifndef _SC10202_pokerNumList_H_
#define _SC10202_pokerNumList_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 发牌、更新当前牌面信息
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10202_pokerNumList : public face2wind::SocketMessage
{
  /**
   * 手上发的牌个数
   */
  unsigned int pokerNum ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt8(pokerNum);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    pokerNum = data->ReadUnsignedInt8();

  }
};  // class end

#endif  //_SC10202_pokerNumList_H_