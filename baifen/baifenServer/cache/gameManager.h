#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <vector>
#include <map>
#include <customData/PokerInfo.h>

namespace face2wind{

  struct PokerVo
  {
    unsigned int color; // 1-4
    unsigned int value; // 1-13
  };

  class GameManager
  {
    std::map<unsigned int, std::vector<unsigned int> > _roomID2PList; // every list only init once on game start
    std::map<unsigned int, unsigned int> _roomID2TurnIndex; // next turn player index
    std::map<unsigned int, std::vector<PokerVo*> *> _playerID2PokerList;

    std::vector<PokerVo*> _rawPokers;

  public:
    GameManager();
    ~GameManager();

    static GameManager *GetInstance();

    void InitGame(const unsigned int &roomID);
    std::vector<unsigned int> *GetPlayerList(const unsigned int &roomID);
    std::vector<PokerVo*> *GetPlayerPokerList(const unsigned int &playerID);
    unsigned int SendCards(const unsigned int &playerID, std::vector<PokerInfo> &pList);
    unsigned int GetCurrentTurnIndex(const unsigned int &roomID){
      if(0 < _roomID2TurnIndex.count(roomID) )
	return _roomID2TurnIndex[roomID];
      else
	return -1;
    }
  };

}

#endif // _GAME_MANAGER_H_
