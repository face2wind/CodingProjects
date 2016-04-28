#ifndef _SC10100_H_
#define _SC10100_H_

#include <socketMessage.h>
#include <byteArray.h>
#include <string>
#include <vector>
#include <customData/RoomInfo.h>


/**
 * 房间列表
 * ( 此文件由工具生成，勿手动修改)
 * @author face2wind
 */
struct SC10100 : public face2wind::SocketMessage
{
  /**
   * 页数
   */
  unsigned int pageNum ;

  /**
   * 最大页数
   */
  unsigned int maxPageNum ;

  /**
   * 房间列表
   */
  std::vector<RoomInfo> roomList ;

  virtual face2wind::ByteArray *PackMsg()
  {
    face2wind::ByteArray *by = new face2wind::ByteArray();
    by->WriteUnsignedInt8(pageNum);
    by->WriteUnsignedInt8(maxPageNum);
    by->WriteUnsignedInt16(roomList.size());
    for (std::vector<RoomInfo>::iterator it = roomList.begin() ; it != roomList.end(); ++it)
      by->ReadFromByteArray( it->PackMsg());
    return by;
  }

  virtual void UnpackMsg(face2wind::ByteArray *data)
  {
    pageNum = data->ReadUnsignedInt8();
    maxPageNum = data->ReadUnsignedInt8();
    int roomListLen = data->ReadUnsignedInt16();
    roomList.clear();
    for (int i = 0; i < roomListLen; ++i){
      RoomInfo tmp_RoomInfo;
      tmp_RoomInfo.UnpackMsg(data);
      roomList.push_back(tmp_RoomInfo);
    }
  }
};  // class end

#endif  //_SC10100_H_