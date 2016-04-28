#ifndef _RoomInfo_H_
#define _RoomInfo_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>
#include <customData/RoomInfo_playerList.h>


/**
 * 房间信息
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct RoomInfo : public face2wind::SocketMessage
{
  /**
   * 房间号
   */
  unsigned int roomID ;

  /**
   * 加锁类型（0无锁，1需要密码，2一律不准进入）
   */
  unsigned int lockType ;

  /**
   * 玩家列表
   */
  std::vector<RoomInfo_playerList> playerList ;

  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt32(roomID);
    by->WriteUnsignedInt8(lockType);
    by->WriteUnsignedInt16(playerList.size());
    for (std::vector<RoomInfo_playerList>::iterator it = playerList.begin() ; it != playerList.end(); ++it)
      by->ReadFromByteArray( it->PackMsg());
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    roomID = data->ReadUnsignedInt32();
    lockType = data->ReadUnsignedInt8();
    int playerListLen = data->ReadUnsignedInt16();
    playerList.clear();
    for (int i = 0; i < playerListLen; ++i){
      RoomInfo_playerList tmp_RoomInfo_playerList;
      tmp_RoomInfo_playerList.UnpackMsg(data);
      playerList.push_back(tmp_RoomInfo_playerList);
    }
  }
};  // class end

#endif  //_RoomInfo_H_