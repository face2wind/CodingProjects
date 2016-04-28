#ifndef _SC10200_H_
#define _SC10200_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 游戏状态
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10200 : public face2wind::SocketMessage
{
  /**
   * 游戏状态，0 结束或未开始，1 进行中
   */
  unsigned int gameState ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt16(gameState);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    gameState = data->ReadUnsignedInt16();

  }
};  // class end

#endif  //_SC10200_H_