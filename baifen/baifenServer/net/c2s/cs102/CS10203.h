#ifndef _CS10203_H_
#define _CS10203_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>
#include <customData/PokerInfo.h>


/**
 * 出牌
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct CS10203 : public face2wind::SocketMessage
{
  /**
   * 出牌列表
   */
  std::vector<PokerInfo> pokerList ;

  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt16(pokerList.size());
    for (std::vector<PokerInfo>::iterator it = pokerList.begin() ; it != pokerList.end(); ++it)
      by->ReadFromByteArray( it->PackMsg());
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    int pokerListLen = data->ReadUnsignedInt16();
    pokerList.clear();
    for (int i = 0; i < pokerListLen; ++i){
      PokerInfo tmp_PokerInfo;
      tmp_PokerInfo.UnpackMsg(data);
      pokerList.push_back(tmp_PokerInfo);
    }
  }
};  // class end

#endif  //_CS10203_H_