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

#include "loginmodule.h"
#include <socketMessage.h>
#include <iostream>
#include <playerManager.h>
#include <roomManager.h>

#include <c2s/cs100/CS10000.h>
#include <c2s/cs100/CS10001.h>
#include <c2s/cs100/CS10002.h>
#include <c2s/cs100/CS10003.h>

#include <s2c/sc100/SC10000.h>
#include <s2c/sc100/SC10001.h>
#include <s2c/sc100/SC10002.h>
#include <s2c/sc100/SC10003.h>
#include <s2c/sc101/SC10103.h>
#include <s2c/sc101/SC10104.h>

#include <customData/RoomInfo.h>
#include <customData/RoomInfo_playerList.h>

LoginModule::LoginModule()
{

}

LoginModule::~LoginModule()
{

}

int LoginModule::OnReceiveMessage(unsigned int cmd,
				  face2wind::SocketMessage *msg,
				  const unsigned int &socketFD)
{
  if(100 != cmd/100) // 100 , login messages
    return 0;
  face2wind::NetworkManager *networkMgr = 
    face2wind::NetworkManager::GetInstance();
  if(10000 == cmd){ // ========================================
    std::cout<<"time stamp = "
	     <<static_cast<CS10000*>(msg)->timeStamp
	     <<std::endl;
    SC10000 *sc10000 = new SC10000();
    sc10000->timeStamp = time(0);
    networkMgr->SendMessage(10000,sc10000,socketFD);
  }else if(10001 == cmd){ // ========================================
    face2wind::PlayerManager *pm = face2wind::PlayerManager::GetInstance();
    face2wind::PlayerVo *vo = 
      pm->GetPlayerWithName(static_cast<CS10002*>(msg)->loginName);
    SC10001 *sc10001 = new SC10001();
    if(NULL == vo)
      sc10001->accountExist = 0;
    else
      sc10001->accountExist = 1;
    networkMgr->SendMessage(10001,sc10001,socketFD);
  }else if(10002 == cmd){ // create account =============================
    face2wind::PlayerManager *pm = face2wind::PlayerManager::GetInstance();
    face2wind::PlayerVo *vo = 
      pm->GetPlayerWithName(static_cast<CS10002*>(msg)->loginName);
    SC10002 *sc10002 = new SC10002();
    if(NULL == vo){
      //std::cout<<"no player name "
      //<<static_cast<CS10002*>(msg)->loginName<<std::endl;
      if(0 == pm->CreatePlayer(static_cast<CS10002*>(msg)->loginName,
			       static_cast<CS10002*>(msg)->password))
	sc10002->errCode = 10000;
      else
	sc10002->errCode = 1;
    }else{
      //std::cout<<"has this player ,can not create it!"<<std::endl;
      sc10002->errCode = 10001;
    }
    networkMgr->SendMessage(10002,sc10002,socketFD);
  }else if(10003 == cmd){ // login ================================
    std::cout<<"login Name = "
	     <<static_cast<CS10002*>(msg)->loginName
	     <<std::endl;
    face2wind::PlayerManager *pm = face2wind::PlayerManager::GetInstance();
    face2wind::PlayerVo *vo = 
      pm->GetPlayerWithName(static_cast<CS10003*>(msg)->loginName);
    SC10003 *sc10003 = new SC10003();
    if(NULL == vo){
      std::cout<<"check no this player !!!!"<<std::endl;
      sc10003->errCode = 10003;
      sc10003->id = 0;
      sc10003->name = "";
    }else{
      //std::cout<<"find player :\nID = "<<vo->id
      //<<"\nName = "<<vo->name<<"\nPassword = "
      //<<vo->password<<std::endl;
      if(static_cast<CS10003*>(msg)->password == vo->password){
	unsigned int curPlayerSocketFD = pm->GetSocketFDWithPlayerID(vo->id);
	if(0 < curPlayerSocketFD) {
	  if(socketFD == curPlayerSocketFD)//_loginPlayerID2Socket[vo->id])
	    return 1;
	  face2wind::RoomManager *roomMgr = face2wind::RoomManager::GetInstance();

	  unsigned int roomID = roomMgr->GetRoomID(vo->id);
	  face2wind::RoomMsg *rMsg = roomMgr->GetRoomMsg(roomID);
	  if(NULL != rMsg){
	    //RoomInfo *rInfo = new RoomInfo();
	    SC10104 *sc4 = new SC10104();
	    sc4->roomInfo.roomID = rMsg->roomID;
	    sc4->roomInfo.lockType = rMsg->lockType;
	    int index = 0;
	    for(std::vector<unsigned int>::iterator pit = rMsg->playerIDList.begin();
		pit != rMsg->playerIDList.end(); ++pit){
	      face2wind::PlayerVo *tmpVo = pm->GetPlayerWithID(*pit);
	      if(0 == *pit || NULL == tmpVo|| vo->id == *pit)
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
	    for(int pIndex = 0; 4 > pIndex; ++pIndex){// notify all room player
	     int roomPlayerID = rMsg->playerIDList[pIndex];
	 if(0 < roomPlayerID){
	int roomPlayerSocketID = pm->GetSocketFDWithPlayerID(roomPlayerID);
	if(rMsg->playerIDList[0] == vo->id){ // leave player is owner, all roomate leave
          SC10103 *sc3 = new SC10103();
          sc3->result = 10109;
          sc3->sitID = pIndex;
          networkMgr->SendMessage(10103,sc3,roomPlayerSocketID);
	}else
	  networkMgr->SendMessage(10104,sc4,roomPlayerSocketID);
	 }
	    }
	  }
	  roomMgr->OutOfRoom(vo->id);
	  pm->RemoveLoginPlayerWithID(vo->id);
	  networkMgr->CloseSocket(curPlayerSocketFD);
	}
	sc10003->errCode = 10002;
	pm->SaveLoginPlayerIDAndSocketFD(vo->id, socketFD);
      }else
	sc10003->errCode = 10004;
      sc10003->id = vo->id;
      sc10003->name = vo->name;
    }
    networkMgr->SendMessage(10003,sc10003,socketFD);
  }
  return 0;
}

int LoginModule::OnConnected(const std::string &ip,
			     const unsigned int &port,
			     const unsigned int &socketFD)
{
  /*
  if(0 < _socket2LoginPlayerID.count(socketFD)){
    int pID = _socket2LoginPlayerID[socketFD];
    _socket2LoginPlayerID.erase(socketFD);
    _loginPlayerID2Socket.erase(pID);
  }
  */
  return 0;
}

int LoginModule::OnAccepted(const unsigned int &socketFD)
{
  //std::cout<<"some one connect -- gateway!"<<std::endl;
  
  //face2wind::NetworkManager *networkMgr =
  //face2wind::NetworkManager::GetInstance();
  //networkMgr->SendMessage(100,t,5);

  return 0;
}

int LoginModule::OnDisconnected(const unsigned int &socketFD)
{
  face2wind::PlayerManager *pm = face2wind::PlayerManager::GetInstance();
  pm->RemoveLoginPlayerWithSocketFD(socketFD);
  return 0;
}
