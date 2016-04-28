/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  dream <email>
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

#include "gamemodule.h"
#include <networkManager.h>
#include <playerManager.h>
#include <roomManager.h>
#include <gameManager.h>

#include <c2s/cs102/CS10203.h>

#include <s2c/sc102/SC10200.h>
#include <s2c/sc102/SC10201.h>
#include <s2c/sc102/SC10202.h>
#include <s2c/sc102/SC10203.h>


GameModule::GameModule()
{

}

GameModule::~GameModule()
{

}

int GameModule::OnReceiveMessage(unsigned int cmd,
				 face2wind::SocketMessage *msg,
				 const unsigned int &socketFD)
{
  if(102 != cmd/100)
    return 1;
  unsigned int subCmd = cmd%102;

  //   face2wind::NetworkManager *networkMgr = 
  //     face2wind::NetworkManager::GetInstance();
  face2wind::PlayerManager *playerMgr
    = face2wind::PlayerManager::GetInstance();
  face2wind::RoomManager *roomMgr = face2wind::RoomManager::GetInstance();
  face2wind::GameManager *gameMgr = face2wind::GameManager::GetInstance();

  unsigned int curPlayerID = playerMgr->GetPlayerIDWithSocketFD(socketFD);
  if(1 == subCmd){ // request start game ================================
    SC10201 *sc1 = new SC10201();
    unsigned int roomID = roomMgr->GetRoomID(curPlayerID);
    face2wind::RoomMsg *rMsg = roomMgr->GetRoomMsg(roomID);
    bool gameStartSucc = false;
    if(0 == roomID)
      sc1->result = 10201;
    else if(1 == rMsg->state)
      sc1->result = 10203;
    else if(roomMgr->GetRoomOwnerID(roomID) == curPlayerID) {
      //cur player is owner, do start game
      sc1->result = 10200;
      roomMgr->setRoomState(roomID,1);
      gameStartSucc = true;
    }else
      sc1->result = 10202;
    networkMgr->SendMessage(10201,sc1,socketFD);

    if(gameStartSucc){
      for(std::vector<unsigned int>::iterator pit = rMsg->playerIDList.begin();
	  pit != rMsg->playerIDList.end(); ++pit){
	SC10200 sc0;// = new SC10200();
	sc0.gameState = 1;
	unsigned int roomPlayerSocketID = playerMgr->GetSocketFDWithPlayerID(*pit);
	if(0 < roomPlayerSocketID)
	  networkMgr->SendMessage(10200,&sc0,roomPlayerSocketID);
      }
    }
    gameMgr->InitGame(roomID);
    for(std::vector<unsigned int>::iterator pit = rMsg->playerIDList.begin();
	pit != rMsg->playerIDList.end(); ++pit){
      SendCurRoomPokersTo(*pit);
    }
  }else if(2 == subCmd){
    SendCurRoomPokersTo(curPlayerID);
  }else if(3 == subCmd){
    CS10203 *cs3 = static_cast<CS10203*>(msg);
    SC10203 *sc3 = new SC10203();
    unsigned int roomID = roomMgr->GetRoomID(curPlayerID);
    std::vector<unsigned int> *roomPlist = gameMgr->GetPlayerList(roomID);
    sc3->curTurnIndex = gameMgr->GetCurrentTurnIndex(roomID);
    //showPlist(cs3->pokerList);
    unsigned int sendCardResult = gameMgr->SendCards(curPlayerID, cs3->pokerList);
    sc3->nextTurnIndex = gameMgr->GetCurrentTurnIndex(roomID);
    sc3->pokerList = cs3->pokerList;
    //std::cout<<"sc3 list:"<<std::endl;
    //showPlist(sc3->pokerList);
    SC10200 sc0;
    sc0.gameState = 0;
    for(std::vector<unsigned int>::iterator it = roomPlist->begin();
	it != roomPlist->end(); ++it){
      if(0 == *it)
	continue;
      unsigned int curPlayerFD = playerMgr->GetSocketFDWithPlayerID(*it);
      networkMgr->SendMessage(10203,sc3,curPlayerFD);
      if(2 == sendCardResult){ //sender has win
	networkMgr->SendMessage(10200,&sc0,curPlayerFD);
      }
    }
    if(2 == sendCardResult){ //sender has win
      roomMgr->setRoomState(roomID,0);
    }
  }
  return 0;
}

int GameModule::OnConnected(const std::string &ip,
			    const unsigned int &port,
			    const unsigned int &socketFD)
{
  return 0;
}

int GameModule::OnAccepted(const unsigned int &socketFD)
{
  return 0;
}

int GameModule::OnDisconnected(const unsigned int &socketFD)
{
 
  return 0;
}

void GameModule::SendCurRoomPokersTo(const unsigned int &playerID)
{
  using face2wind::PokerVo;
  using face2wind::RoomManager;
  using face2wind::PlayerManager;
  using face2wind::GameManager;

  PlayerManager *playerMgr = PlayerManager::GetInstance();
  RoomManager *roomMgr = RoomManager::GetInstance();
  GameManager *gameMgr = GameManager::GetInstance();

  unsigned int roomID = roomMgr->GetRoomID(playerID);
  if(0 == roomID)
    return;
  std::vector<unsigned int> *roomPlist = gameMgr->GetPlayerList(roomID);
  SC10202 *sc2 = new SC10202();
  sc2->nextTurnIndex = gameMgr->GetCurrentTurnIndex(roomID);
  unsigned int index = 0;
  for(std::vector<unsigned int>::iterator pid = roomPlist->begin();
      pid != roomPlist->end(); ++pid){
    std::vector<PokerVo*> *pokerList = gameMgr->GetPlayerPokerList(*pid);
    SC10202_pokerNumList num;
    if(NULL != pokerList)
      num.pokerNum = pokerList->size();
    else
      num.pokerNum = 0;
    sc2->pokerNumList.push_back(num);
    if(playerID == *pid && NULL != pokerList){
      sc2->roleIndex = index;
      for(std::vector<PokerVo*>::iterator pvo = pokerList->begin();
	  pvo != pokerList->end(); ++pvo){
	PokerInfo info;
	info.color = (*pvo)->color;
	info.value = (*pvo)->value;
	sc2->pokerList.push_back(info);
      }
    }
    ++index;
  }
  unsigned int roomPlayerSocketID = playerMgr->GetSocketFDWithPlayerID(playerID);
  networkMgr->SendMessage(10202,sc2,roomPlayerSocketID);
}
