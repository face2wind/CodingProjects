#include <roomManager.h>
#include <string>
#include <vector>
#include <list>
#include <stack>


namespace face2wind {

  RoomManager *RoomManager::GetInstance() {
    static RoomManager m;
    return &m;
  }

  std::vector<RoomMsg*> RoomManager::GetRoomListWithPage(unsigned int page)
  {
    unsigned int allRoomNum = roomList.size();
    if(allRoomNum < (page-1)*_ONE_PAGE_ROOM_NUM){//not enough
      std::vector<RoomMsg*> tmp;
      return tmp;
    }
    unsigned int roomNum = _ONE_PAGE_ROOM_NUM;
    if(allRoomNum < page*_ONE_PAGE_ROOM_NUM)
      roomNum = allRoomNum - (page-1)*_ONE_PAGE_ROOM_NUM;
    unsigned int startIndex = (page-1)*_ONE_PAGE_ROOM_NUM;
    unsigned int endIndex = startIndex + roomNum;
    std::vector<RoomMsg*> rlist(roomNum);
    unsigned int i = 0;
    for(std::list<RoomMsg*>::iterator it = roomList.begin();
	it != roomList.end() && i < endIndex; ++it, ++i){
      if(startIndex <= i){
	rlist[i-startIndex] = *it;
      }
    }
    return rlist;
  }

  RoomMsg *RoomManager::GetRoomMsg(unsigned int roomID)
  {
    for(std::list<RoomMsg*>::iterator it = roomList.begin();
	it != roomList.end(); ++it){
      if((*it)->roomID == roomID)
	return *it;
    }
    return NULL;
  }

  unsigned int RoomManager::GetMaxPageNum()
  {
    unsigned int allRoomNum = roomList.size();
    unsigned int lastPageRoomNum = allRoomNum%_ONE_PAGE_ROOM_NUM;
    unsigned int maxPageNum = allRoomNum/_ONE_PAGE_ROOM_NUM;
    if(0 < lastPageRoomNum)
      ++ maxPageNum;
    return maxPageNum;
  }

  int RoomManager::CreateRoom(const unsigned int &playerID,
			      const unsigned int &lockType,
			      const std::string &password)
  {
    if(1 > playerID)
      return -1;
    if(0 < playerID2RoomID.count(playerID)) //this player has a room
      return -2;
    unsigned int roomID = GetNewRoomID();
    RoomMsg *room = new RoomMsg(roomID,lockType,playerID,password);
    roomList.push_back(room);
    playerID2RoomID[playerID] = roomID;
    roomID2OwnerID[roomID] = playerID;
    return roomID;
  }

  int RoomManager::JoinRoom(const unsigned int &roomID,
			    const unsigned int &playerID,
			    const unsigned int &sitID,
			    const std::string &passwd)
  {
    if(0 > sitID || 3 < sitID)
      return -1;
    RoomMsg *room = NULL;
    for(std::list<RoomMsg*>::iterator it = roomList.begin();
	it != roomList.end(); ++it){
      if((*it)->roomID == roomID){
	room = *it;
	break;
      }
    }
    if(NULL == room) // no this room
      return -4;
    if(0 != room->playerIDList[sitID]) //sit not empty
      return -2;
    if(room->password != passwd)
      return -3;
    room->playerIDList[sitID] = playerID;
    playerID2RoomID[playerID] = room->roomID;
    return room->roomID;
  }

  unsigned int RoomManager::OutOfRoom(const unsigned int &roomID,
			     const unsigned int &playerID,
			     const unsigned int &sitID)
  {
    if(0 > sitID || 3 < sitID)
      return 2;
    RoomMsg *room = NULL;
    for(std::list<RoomMsg*>::iterator it = roomList.begin();
	it != roomList.end(); ++it){
      if((*it)->roomID == roomID){
	room = *it;

	if(0 == room->playerIDList[sitID]) //sit is empty
	  return 3;
	if(playerID != room->playerIDList[sitID])// sit do not match player
	  return 4;
	if(1 == room->state) // room are playing
	  return 6;
	room->playerIDList[sitID] = 0;
	playerID2RoomID.erase(playerID);
	if(0 == sitID){ // who out is owner, dismiss the room
	  roomID2OwnerID.erase(roomID);
	  freeRoomIDs.push(roomID);
	  for(std::vector<unsigned int>::iterator pid = room->playerIDList.begin();
	      pid != room->playerIDList.end(); ++pid){
	    if(0 != (*pid)) // this sit has user
	      playerID2RoomID.erase(*pid);
	  }
	  roomList.erase(it);
	  delete *it;
	  return 5;
	}
	return 0;
      }
    }
    return 1;
  }

  unsigned int RoomManager::OutOfRoom(const unsigned int &playerID)
  {
    unsigned int roomID = GetRoomID(playerID);
    
    RoomMsg *room = NULL;
    for(std::list<RoomMsg*>::iterator it = roomList.begin();
	it != roomList.end(); ++it){
      if((*it)->roomID == roomID){
	room = *it;

	unsigned int sitID = -1;
	for(unsigned int i = 0; i < 4 ; ++i)
	  if(room->playerIDList[i] == playerID){
	    sitID = i;
	    room->playerIDList[i] = 0;
	  }
	//if(-1 == roomID)
	//break;
	//room->playerIDList[sitID] = 0;
	playerID2RoomID.erase(playerID);
	if(0 == sitID){ // who out is owner, dismiss the room
	  roomID2OwnerID.erase(roomID);
	  freeRoomIDs.push(roomID);
	  for(std::vector<unsigned int>::iterator pid = room->playerIDList.begin();
	      pid != room->playerIDList.end(); ++pid){
	    if(0 != (*pid)) // this sit has user
	      playerID2RoomID.erase(*pid);
	  }

	  roomList.erase(it);
	  delete *it;
	  return 5;
	}
	return 0;
      }
    }
    return 0;
  }

  unsigned int RoomManager::setRoomState(const unsigned int &roomID, const unsigned int &state)
  {
    RoomMsg *room = NULL;
    for(std::list<RoomMsg*>::iterator it = roomList.begin();
	it != roomList.end(); ++it){
      if((*it)->roomID == roomID){
	room = *it;
	room->state = state;
	break;
      }
    }
    return 0;
  }

}
