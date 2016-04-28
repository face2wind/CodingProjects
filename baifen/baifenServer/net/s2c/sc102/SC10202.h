#ifndef _SC10202_H_
#define _SC10202_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>
#include <customData/SC10202_pokerNumList.h>
#include <customData/PokerInfo.h>


/**
 * 发牌、更新当前牌面信息
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10202 : public face2wind::SocketMessage
{
  /**
   * 自己的下标
   */
  unsigned int roleIndex ;

  /**
   * 下一个出牌的玩家下标
   */
  unsigned int nextTurnIndex ;

  /**
   * 所有人手上的牌个数
   */
  std::vector<SC10202_pokerNumList> pokerNumList ;
  /**
   * 自己手上的牌
   */
  std::vector<PokerInfo> pokerList ;

  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt8(roleIndex);
    by->WriteUnsignedInt8(nextTurnIndex);
    by->WriteUnsignedInt16(pokerNumList.size());
    for (std::vector<SC10202_pokerNumList>::iterator it = pokerNumList.begin() ; it != pokerNumList.end(); ++it)
      by->ReadFromByteArray( it->PackMsg());
    by->WriteUnsignedInt16(pokerList.size());
    for (std::vector<PokerInfo>::iterator it = pokerList.begin() ; it != pokerList.end(); ++it)
      by->ReadFromByteArray( it->PackMsg());
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    roleIndex = data->ReadUnsignedInt8();
    nextTurnIndex = data->ReadUnsignedInt8();
    int pokerNumListLen = data->ReadUnsignedInt16();
    pokerNumList.clear();
    for (int i = 0; i < pokerNumListLen; ++i){
      SC10202_pokerNumList tmp_SC10202_pokerNumList;
      tmp_SC10202_pokerNumList.UnpackMsg(data);
      pokerNumList.push_back(tmp_SC10202_pokerNumList);
    }    int pokerListLen = data->ReadUnsignedInt16();
    pokerList.clear();
    for (int i = 0; i < pokerListLen; ++i){
      PokerInfo tmp_PokerInfo;
      tmp_PokerInfo.UnpackMsg(data);
      pokerList.push_back(tmp_PokerInfo);
    }
  }
};  // class end

#endif  //_SC10202_H_