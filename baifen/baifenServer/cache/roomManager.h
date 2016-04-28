#ifndef _ROOM_MANAGER_H_
#define _ROOM_MANAGER_H_

#include <string>
#include <vector>
#include <list>
#include <stack>
#include <map>

namespace face2wind {

  struct RoomMsg
  {
    unsigned int roomID;
    unsigned int lockType; // 0 no lock, 1 need password, 2 can not join
    unsigned int state; // 0 no start, 1 playing
    std::string password;
    std::vector<unsigned int> playerIDList;
    
  RoomMsg(const unsigned int &_rID,
	  const unsigned int &_lType,
	  const unsigned int &_ownerID,
	  const std::string &_password)
  :roomID(_rID),lockType(_lType),state(0),
      password(_password),playerIDList(4){
    playerIDList[0] = _ownerID;
    playerIDList[1] = 0;
    playerIDList[2] = 0;
    playerIDList[3] = 0;
  }
    ~RoomMsg(){}
    
  };

  class RoomManager
  {
    std::list<RoomMsg*> roomList;
    std::stack<RoomMsg*> roomCache; // class object cache

    std::map<unsigned int,unsigned int> playerID2RoomID;
    std::map<unsigned int,unsigned int> roomID2OwnerID;

    unsigned int _ONE_PAGE_ROOM_NUM = 6;

    unsigned int nextMaxID;
    std::stack<unsigned int> freeRoomIDs;
    unsigned int GetNewRoomID(){
       unsigned int targetID = 0;
       if(freeRoomIDs.empty())
	 targetID = nextMaxID++;
       else{
         targetID = freeRoomIDs.top();
	 freeRoomIDs.pop();
       }
       return targetID;
  }

  public:
    RoomManager(){
    nextMaxID = 1;
  }
    ~RoomManager(){}
    unsigned int GetRoomOwnerID(unsigned int roomID){
    if(0 < roomID2OwnerID.count(roomID))
      return roomID2OwnerID[roomID];
    return 0;
  }
    unsigned int GetRoomID(unsigned int playerID){ // get room id which player in there
    if(0 < playerID2RoomID.count(playerID))
      return playerID2RoomID[playerID];
    return 0;
  }

    static RoomManager *GetInstance();

    std::vector<RoomMsg*> GetRoomListWithPage(unsigned int page);
    RoomMsg *GetRoomMsg(unsigned int roomID);
    unsigned int GetMaxPageNum();
    int CreateRoom(const unsigned int &playerID,
      const unsigned int &lockType,
      const std::string &password);
    int JoinRoom(const unsigned int &roomID,const unsigned int &playerID,const unsigned int &sitID,
		 const std::string &passwd);
    unsigned int OutOfRoom(const unsigned int &roomID,const unsigned int &playerID,const unsigned int &sitID);
    unsigned int OutOfRoom(const unsigned int &playerID);
    
    unsigned int setRoomState(const unsigned int &roomID, const unsigned int &state);
  };

  }

#endif // _ROOM_MANAGER_H_
