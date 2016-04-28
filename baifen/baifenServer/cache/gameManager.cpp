#include <gameManager.h>
#include <roomManager.h>
#include <vector>
#include <map>

#include <iostream>

namespace face2wind{

  GameManager *GameManager::GetInstance() {
    static GameManager m;
    return &m;
  }
  
  GameManager::GameManager()
  {
    PokerVo *vo = NULL;
    for(unsigned int i = 1; 14 > i ; ++i)
      for(unsigned int j = 1; 5 > j ; ++j){
	PokerVo *vo = new PokerVo();
	vo->color = j;
	vo->value = i;
	_rawPokers.push_back(vo);
      }
    vo = new PokerVo();
    vo->color = 1;
    vo->value = 14;
    _rawPokers.push_back(vo);

    vo = new PokerVo();
    vo->color = 2;
    vo->value = 14;
    _rawPokers.push_back(vo);
  }

  GameManager::~GameManager()
  {
    for(std::vector<PokerVo*>::iterator it = _rawPokers.begin();
	it != _rawPokers.end(); ++it){
      delete (*it);
    }
  }

  void GameManager::InitGame(const unsigned int &roomID)
  {
    RoomMsg *rMsg = RoomManager::GetInstance()->GetRoomMsg(roomID);
    if(NULL == rMsg)
      return;

    _roomID2PList[roomID] = rMsg->playerIDList;
    std::vector<PokerVo*> tmpPlist = _rawPokers;
    std::vector<unsigned int> pidList;
    std::vector<unsigned int> pidIndexList;
    unsigned int tmpPID = _roomID2PList[roomID][0];
    if(0 < tmpPID){
      pidList.push_back(tmpPID);
      pidIndexList.push_back(0);
      if(NULL == _playerID2PokerList[tmpPID])
	_playerID2PokerList[tmpPID] = new std::vector<PokerVo*>();
      else
	_playerID2PokerList[tmpPID]->clear();
    }
    tmpPID = _roomID2PList[roomID][1];
    if(0 < tmpPID){
      pidList.push_back(tmpPID);
      pidIndexList.push_back(1);
      if(NULL == _playerID2PokerList[tmpPID])
	_playerID2PokerList[tmpPID] = new std::vector<PokerVo*>();
      else
	_playerID2PokerList[tmpPID]->clear();
    }
    tmpPID = _roomID2PList[roomID][2];
    pidIndexList.push_back(2);
    if(0 < tmpPID){
      pidList.push_back(tmpPID);
      if(NULL == _playerID2PokerList[tmpPID])
	_playerID2PokerList[tmpPID] = new std::vector<PokerVo*>();
      else
	_playerID2PokerList[tmpPID]->clear();
    }
  
    tmpPID = _roomID2PList[roomID][3];
    if(0 < tmpPID){
      pidList.push_back(tmpPID);
      pidIndexList.push_back(3);
      if(NULL == _playerID2PokerList[tmpPID])
	_playerID2PokerList[tmpPID] = new std::vector<PokerVo*>();
      else
	_playerID2PokerList[tmpPID]->clear();
    }
    std::vector<unsigned int>::iterator pid = pidList.begin();
    std::vector<unsigned int>::iterator pidIndex = pidIndexList.begin();
    for(std::vector<PokerVo*>::iterator poker = tmpPlist.begin();
	poker != tmpPlist.end(); ++ poker){
      if(3 == (*poker)->value &&
	 1 == (*poker)->color)
	_roomID2TurnIndex[roomID] = *pidIndex;
      _playerID2PokerList[*pid]->push_back(*poker);
      ++pid;
      ++pidIndex;
      if(pid == pidList.end()){
	pid = pidList.begin();
	pidIndex = pidIndexList.begin();
      }
    }
  }

  std::vector<unsigned int> *GameManager::GetPlayerList(const unsigned int &roomID)
  {
    if(0 < _roomID2PList.count(roomID))
      return &_roomID2PList[roomID];
    else
      return NULL;
  }

  std::vector<PokerVo*> *GameManager::GetPlayerPokerList(const unsigned int &playerID)
  {
    if(0 < _playerID2PokerList.count(playerID))
      return _playerID2PokerList[playerID];
    else
      return NULL;
  }

  unsigned int GameManager::SendCards(const unsigned int &playerID,
			      std::vector<PokerInfo> &pList)
  {
    if(0 == _playerID2PokerList.count(playerID))
      return 1;
    std::vector<PokerVo*> *pokerList = _playerID2PokerList[playerID];
    std::vector<PokerVo*> *newPokerList = new std::vector<PokerVo*>();
    for(std::vector<PokerVo*>::iterator it = pokerList->begin();
	it != pokerList->end(); ++it){
      PokerVo *pvo = *it;
      bool hasFound = false;
      for(std::vector<PokerInfo>::iterator pInfo = pList.begin();
	  pInfo != pList.end(); ++pInfo){
	if((*pInfo).color == pvo->color &&
	   (*pInfo).value == pvo->value){
	  hasFound = true;
	  break;
	}
      }
      if(!hasFound)
	newPokerList->push_back(pvo);
    }
    _playerID2PokerList[playerID] = newPokerList;
    using face2wind::RoomManager;
    RoomManager *roomMgr = RoomManager::GetInstance();
    unsigned int roomID = roomMgr->GetRoomID(playerID);
    unsigned int index = _roomID2TurnIndex[roomID];
    int newIndex = -1;
    std::vector<unsigned int> *roomPlist = GetPlayerList(roomID);
    // find next turn index
    for(unsigned int i = index+1; i < 4; i++)
      if(0 != (*roomPlist)[i]){
	newIndex = (int)i;
	break;
      }
    if(-1 == newIndex){
      for(unsigned int i = 0; i < 4; i++)
	if(0 != (*roomPlist)[i]){
	  newIndex = (int)i;
	  break;
	}
    }
    _roomID2TurnIndex[roomID] = (unsigned int)newIndex;
    if(0 == newPokerList->size()) // the player which send card has win
      return 2;
    return 0;
  }

}
