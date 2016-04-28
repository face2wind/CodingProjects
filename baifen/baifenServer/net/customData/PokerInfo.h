#ifndef _PokerInfo_H_
#define _PokerInfo_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 扑克牌信息
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct PokerInfo : public face2wind::SocketMessage
{
  /**
   * 花色（1-4，砖块、花、红桃、黑桃）
   */
  unsigned int color ;

  /**
   * 牌值（1-13）
   */
  unsigned int value ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt8(color);
    by->WriteUnsignedInt8(value);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    color = data->ReadUnsignedInt8();
    value = data->ReadUnsignedInt8();

  }
};  // class end

#endif  //_PokerInfo_H_