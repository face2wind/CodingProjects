/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  dream <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <baifenmodule.h>
#include <socketMessage.h>
#include <iostream>
#include <playerManager.h>
#include <roomManager.h>
#include <vector>

#include <c2s/cs100/CS10000.h>
#include <c2s/cs100/CS10001.h>
#include <c2s/cs100/CS10002.h>
#include <c2s/cs100/CS10003.h>
#include <c2s/cs101/CS10100.h>
#include <c2s/cs101/CS10101.h>
#include <c2s/cs101/CS10102.h>
#include <c2s/cs101/CS10103.h>

#include <s2c/sc100/SC10000.h>
#include <s2c/sc100/SC10001.h>
#include <s2c/sc100/SC10002.h>
#include <s2c/sc100/SC10003.h>
#include <s2c/sc101/SC10100.h>
#include <s2c/sc101/SC10101.h>
#include <s2c/sc101/SC10102.h>
#include <s2c/sc101/SC10103.h>
#include <s2c/sc101/SC10104.h>

#include <customData/RoomInfo.h>
#include <customData/RoomInfo_playerList.h>

BaifenModule::BaifenModule()
{

}

BaifenModule::~BaifenModule()
{

}

int BaifenModule::OnReceiveMessage(unsigned int cmd,
				  face2wind::SocketMessage *msg,
				  const unsigned int &socketFD)
{
  if(101 != cmd/100)
    return 1;
  unsigned int subCmd = cmd%101;

  face2wind::NetworkManager *networkMgr = 
    face2wind::NetworkManager::GetInstance();
  face2wind::PlayerManager *playerMgr
    = face2wind::PlayerManager::GetInstance();
  face2wind::RoomManager *roomMgr = face2wind::RoomManager::GetInstance();

  if(0 == subCmd){ // get room list ================================
    CS10100 *cs0 = static_cast<CS10100*>(msg);
    std::vector<face2wind::RoomMsg*> rlist =
      roomMgr->GetRoomListWithPage(cs0->pageNum);
    SC10100 *sc0 = new SC10100();
    sc0->pageNum = cs0->pageNum;
    sc0->maxPageNum = roomMgr->GetMaxPageNum();
    for(std::vector<face2wind::RoomMsg*>::iterator it = rlist.begin();
	it != rlist.end(); ++it){
      RoomInfo rInfo;// = new RoomInfo();
      //face2wind::RoomMsg *rMsg = static_cast<face2wind::RoomMsg*>(*it);
      rInfo.roomID = (*it)->roomID;
      rInfo.lockType = (*it)->lockType;
      unsigned int index = 0;
      for(std::vector<unsigned int>::iterator pit = (*it)->playerIDList.begin();
	  pit != (*it)->playerIDList.end(); ++pit){
	face2wind::PlayerVo *tmpVo = playerMgr->GetPlayerWithID(*pit);
	if(0 == *pit || NULL == tmpVo)
	  continue;
	RoomInfo_playerList plist;// = new RoomInfo_playerList();
	plist.playerID = tmpVo->id;
	plist.playerName = tmpVo->name;
	plist.sitID = index;
	plist.isOwner = (0 == index?
			  1:0 );
	std::cout<<"room one player : "<<index<<" - "<<tmpVo->name<<std::endl;
	++index;
	rInfo.playerList.push_back(plist);
      }
      sc0->roomList.push_back(rInfo);
    }
    networkMgr->SendMessage(10100,sc0,socketFD);
  }else if(1 == subCmd){ // join room ================================
    CS10101 *cs1 = static_cast<CS10101*>(msg);
    SC10101 *sc1 = new SC10101();
    unsigned int pid = playerMgr->GetPlayerIDWithSocketFD(socketFD);
    int joinResult = 
      roomMgr->JoinRoom(cs1->roomID,pid,cs1->sitID,cs1->passwd);
    sc1->roomID = joinResult;
    if(0 < joinResult)
      sc1->result = 10100;
    else if(-1 == joinResult)
      sc1->result = 10103;
    else if(-2 == joinResult)
      sc1->result = 10101;
    else if(-3 == joinResult)
      sc1->result = 10102;
    else if(-4 == joinResult)
      sc1->result = 10104;
    else
      sc1->result = 1;
    networkMgr->SendMessage(10101,sc1,socketFD);

    face2wind::RoomMsg *rMsg = roomMgr->GetRoomMsg(joinResult);
    if(NULL != rMsg){
      //RoomInfo *rInfo = new RoomInfo();
      SC10104 *sc4 = new SC10104();
      sc4->roomInfo.roomID = rMsg->roomID;
      sc4->roomInfo.lockType = rMsg->lockType;
      unsigned int index = 0;
      for(std::vector<unsigned int>::iterator pit = rMsg->playerIDList.begin();
	  pit != rMsg->playerIDList.end(); ++pit){
	face2wind::PlayerVo *tmpVo = playerMgr->GetPlayerWithID(*pit);
	if(0 == *pit || NULL == tmpVo)
	  continue;
	RoomInfo_playerList plist;// = new RoomInfo_playerList();
	plist.playerID = tmpVo->id;
	plist.playerName = tmpVo->name;
	plist.sitID = index;
	plist.isOwner = (0 == index?
			  1:0 );
	std::cout<<"room one player : "<<index<<" - "<<tmpVo->name<<std::endl;
	++index;
	sc4->roomInfo.playerList.push_back(plist);
      }
      for(unsigned int pIndex = 0; 4 > pIndex; ++pIndex){// notify all room player
	unsigned int roomPlayerID = rMsg->playerIDList[pIndex];
	 if(0 < roomPlayerID){
	unsigned int roomPlayerSocketID = playerMgr->GetSocketFDWithPlayerID(roomPlayerID);
	networkMgr->SendMessage(10104,sc4,roomPlayerSocketID);
	 }
      }
    }
  }else if(2 == subCmd){ // create room =============================
    CS10102 *cs2 = static_cast<CS10102*>(msg);
    unsigned int pid = playerMgr->GetPlayerIDWithSocketFD(socketFD);
    int roomID = roomMgr->CreateRoom(pid,cs2->lockType, cs2->passwd);
    SC10102 *sc2 = new SC10102();
    sc2->roomID = (unsigned int)roomID;
    if(0 < roomID)
      sc2->result = 10105;
    else if(-2 == roomID) //aleady has a room
      sc2->result = 10108;
    else{
      std::cout<<"create room return : "<<roomID<<std::endl;
      sc2->result = 1;
    }
    networkMgr->SendMessage(10102,sc2,socketFD);
  }else if(3 == subCmd){ // out of room =============================
    CS10103 *cs3 = static_cast<CS10103*>(msg);
    unsigned int playerID = playerMgr->GetPlayerIDWithSocketFD(socketFD);
    face2wind::RoomMsg *rMsg = roomMgr->GetRoomMsg(cs3->roomID);
    SC10103 *sc3 = new SC10103();
    if(1 == rMsg->state){
      sc3->result = 10110;
      sc3->sitID = cs3->sitID;
      networkMgr->SendMessage(10103,sc3,socketFD);
      return 2;
    }else if(NULL != rMsg){
      sc3->result = 10109;
      //RoomInfo *rInfo = new RoomInfo();
      SC10104 *sc4 = new SC10104();
      sc4->roomInfo.roomID = rMsg->roomID;
      sc4->roomInfo.lockType = rMsg->lockType;
      unsigned int index = 0;
      for(std::vector<unsigned int>::iterator pit = rMsg->playerIDList.begin();
	  pit != rMsg->playerIDList.end(); ++pit){
	face2wind::PlayerVo *tmpVo = playerMgr->GetPlayerWithID(*pit);
	if(0 == *pit || NULL == tmpVo|| playerID == *pit)
	  continue;
	RoomInfo_playerList plist;// = new RoomInfo_playerList();
	plist.playerID = tmpVo->id;
	plist.playerName = tmpVo->name;
	plist.sitID = index;
	plist.isOwner = (0 == index?1:0 );
	std::cout<<"room one player : "<<index<<" - "<<tmpVo->name<<std::endl;
	++index;
	sc4->roomInfo.playerList.push_back(plist);
      }
      for(unsigned int pIndex = 0; 4 > pIndex; ++pIndex){// notify all room player
	unsigned int roomPlayerID = rMsg->playerIDList[pIndex];
	 if(0 < roomPlayerID){
	unsigned int roomPlayerSocketID = playerMgr->GetSocketFDWithPlayerID(roomPlayerID);
	if(rMsg->playerIDList[0] == playerID){ // leave player is owner, all roomate leave
          sc3->sitID = pIndex;
          networkMgr->SendMessage(10103,sc3,roomPlayerSocketID);
	}else
	  networkMgr->SendMessage(10104,sc4,roomPlayerSocketID);
	 }
      }
    }
    roomMgr->OutOfRoom(cs3->roomID,
		       playerMgr->GetPlayerIDWithSocketFD(socketFD),
		       cs3->sitID);
    sc3->sitID = cs3->sitID;
    networkMgr->SendMessage(10103,sc3,socketFD);
  }
  return 0;
}

int BaifenModule::OnConnected(const std::string &ip,
			     const unsigned int &port,
			     const unsigned int &socketFD)
{
  return 0;
}

int BaifenModule::OnAccepted(const unsigned int &socketFD)
{
  return 0;
}

int BaifenModule::OnDisconnected(const unsigned int &socketFD)
{
  face2wind::NetworkManager *networkMgr = 
    face2wind::NetworkManager::GetInstance();
  face2wind::PlayerManager *playerMgr
    = face2wind::PlayerManager::GetInstance();
  face2wind::RoomManager *roomMgr = face2wind::RoomManager::GetInstance();
  unsigned int playerID = playerMgr->GetPlayerIDWithSocketFD(socketFD);

  unsigned int roomID = roomMgr->GetRoomID(playerID);
  face2wind::RoomMsg *rMsg = roomMgr->GetRoomMsg(roomID);
  if(NULL != rMsg){
    //RoomInfo *rInfo = new RoomInfo();
    SC10104 *sc4 = new SC10104();
    sc4->roomInfo.roomID = rMsg->roomID;
    sc4->roomInfo.lockType = rMsg->lockType;
    unsigned int index = 0;
    for(std::vector<unsigned int>::iterator pit = rMsg->playerIDList.begin();
	pit != rMsg->playerIDList.end(); ++pit){
      face2wind::PlayerVo *tmpVo = playerMgr->GetPlayerWithID(*pit);
      if(0 == *pit || NULL == tmpVo|| playerID == *pit)
	continue;
      RoomInfo_playerList plist;// = new RoomInfo_playerList();
      plist.playerID = tmpVo->id;
      plist.playerName = tmpVo->name;
      plist.sitID = index;
      plist.isOwner = (0 == index?
			1:0 );
      std::cout<<"room one player : "<<index<<" - "<<tmpVo->name<<std::endl;
      ++index;
      sc4->roomInfo.playerList.push_back(plist);
    }
    for(unsigned int pIndex = 0; 4 > pIndex; ++pIndex){// notify all room player
      unsigned int roomPlayerID = rMsg->playerIDList[pIndex];
	 if(0 < roomPlayerID){
	unsigned int roomPlayerSocketID = playerMgr->GetSocketFDWithPlayerID(roomPlayerID);
	if(rMsg->playerIDList[0] == playerID){ // leave player is owner, all roomate leave
          SC10103 *sc3 = new SC10103();
          sc3->result = 10109;
          sc3->sitID = pIndex;
          networkMgr->SendMessage(10103,sc3,roomPlayerSocketID);
	}else
	  networkMgr->SendMessage(10104,sc4,roomPlayerSocketID);
	 }
    }
  }
  roomMgr->OutOfRoom(playerID);
  return 0;
}

int BaifenModule::SendRoomMsgTo(const unsigned int &socketFD,
				face2wind::RoomMsg *roomMsg )
{
  
  return 0;
}
