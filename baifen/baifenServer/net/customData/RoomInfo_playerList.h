#ifndef _RoomInfo_playerList_H_
#define _RoomInfo_playerList_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>


/**
 * 房间信息
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct RoomInfo_playerList : public face2wind::SocketMessage
{
  /**
   * 玩家ID
   */
  unsigned int playerID ;

  /**
   * 玩家名
   */
  std::string playerName ;

  /**
   * 座位ID（0-3）
   */
  unsigned int sitID ;

  /**
   * 是否房主（1是，0否）
   */
  unsigned int isOwner ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt32(playerID);
    by->WriteString(playerName);
    by->WriteUnsignedInt8(sitID);
    by->WriteUnsignedInt8(isOwner);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    playerID = data->ReadUnsignedInt32();
    playerName = data->ReadString();
    sitID = data->ReadUnsignedInt8();
    isOwner = data->ReadUnsignedInt8();

  }
};  // class end

#endif  //_RoomInfo_playerList_H_