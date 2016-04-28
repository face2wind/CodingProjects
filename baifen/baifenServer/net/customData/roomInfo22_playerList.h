#ifndef _roomInfo22_H_
#define _roomInfo22_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>
#include <customData/roomInfo22_playerList.h>


/**
 * 房间信息
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct roomInfo22 : public face2wind::SocketMessage
{
  /**
   * 房间号
   */
  long romeID ;

  /**
   * 加锁类型（0无锁，1需要密码，2一律不准进入）
   */
  long lockType ;

  /**
   * 玩家列表
   */
  std::vector<roomInfo22_playerList*> playerList ;
  /**
   * 是否房主（1是，0否）
   */
  long isOwner ;


  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteLong(romeID);
    by->WriteChar(lockType);
    for (std::vector<roomInfo22_playerList*>::iterator it = playerList.begin() ; it != playerList.end(); ++it)
      by->ReadFromByteArray( (*it)->PackMsg() );
    by->WriteChar(isOwner);
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    romeID = data->ReadLong();
    lockType = data->ReadChar();
    int playerListLen = data->ReadShort();
    for (int i = 0; i < playerListLen; ++i){
      roomInfo22_playerList *tmp_roomInfo22_playerList = new roomInfo22_playerList();
      tmp_roomInfo22_playerList->UnpackMsg(data);
      testArr.push_back(tmp_roomInfo22_playerList);
    }    isOwner = data->ReadChar();

  }
};  // class end

#endif  //_roomInfo22_playerList_H_