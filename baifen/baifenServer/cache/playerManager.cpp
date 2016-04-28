#include <playerManager.h>
#include <baifenSql.h>
#include <string>
#include <iostream>

namespace face2wind {

  PlayerManager *PlayerManager::GetInstance() {
    static PlayerManager m;
    return &m;
  }

  PlayerVo *PlayerManager::GetPlayerWithName(const std::string &name)
  {
    if(0 < _name2voCache.count(name)){
      //std::cout<<"found playervo in cache"<<std::endl;
      return _name2voCache[name];
    }
    //std::cout<<"can not found playervo in cache, read mysql"<<std::endl;
    SqlResult *result = sql->Query("select * from Player where name='"
				   +name+"'");
    if(NULL == result)
      return NULL;
    unsigned int len = result->Size();
    for(unsigned int i = 0; i < len; ++i){
      PlayerVo *vo = new PlayerVo();
      SqlResultRow *row = result->GetRow(i);
      vo->id = row->ReadInt(0);
      vo->name = row->ReadString(1);
      vo->password = row->ReadString(2);
      _name2voCache[name] = vo;
      _id2voCache[vo->id] = vo;
    }
    delete result;
    if(0 < _name2voCache.count(name)){
      return _name2voCache[name];
    }
    return NULL;
  }

  PlayerVo *PlayerManager::GetPlayerWithID(const unsigned int &id)
  {
    if(0 < _id2voCache.count(id)){
      //std::cout<<"found playervo in cache"<<std::endl;
      return _id2voCache[id];
    }
    //std::cout<<"can not found playervo in cache, read mysql"<<std::endl;
    SqlResult *result = sql->Query("select * from Player where id='"
				   +std::to_string(id)+"'");
    if(NULL == result)
      return NULL;
    unsigned int len = result->Size();
    for(unsigned int i = 0; i < len; ++i){
      PlayerVo *vo = new PlayerVo();
      SqlResultRow *row = result->GetRow(i);
      vo->id = row->ReadInt(0);
      vo->name = row->ReadString(1);
      vo->password = row->ReadString(2);
      _name2voCache[vo->name] = vo;
      _id2voCache[id] = vo;
    }
    delete result;
    if(0 < _id2voCache.count(id)){
      return _id2voCache[id];
    }
    return NULL;
  }

  unsigned int PlayerManager::CreatePlayer(const std::string &name,
				  const std::string &passwd)
  {
    if(NULL == 
       sql->Query("INSERT UNSIGNED INTO Player VALUES(null,"+name+","+passwd+");"))
      return 1;
    return 0;
  }

  bool PlayerManager::ChangePlayerState(const std::string &name)
  {
    return false;
  }

  unsigned int PlayerManager::GetPlayerIDWithSocketFD(unsigned int socketFD)
  {
    if(0 < _socket2LoginPlayerID.count(socketFD))
      return _socket2LoginPlayerID[socketFD];
    return 0;
  }

  unsigned int PlayerManager::GetSocketFDWithPlayerID(unsigned int playerID)
  {
    if(0 < _loginPlayerID2Socket.count(playerID))
      return _loginPlayerID2Socket[playerID];
    return 0;
  }
   
  void PlayerManager::SaveLoginPlayerIDAndSocketFD(unsigned int playerID,
						   unsigned int socketFD)
  {
    _socket2LoginPlayerID[socketFD] = playerID;
    _loginPlayerID2Socket[playerID] = socketFD;
  }
   
  void PlayerManager::RemoveLoginPlayerWithID(unsigned int playerID)
  {
    if(0 == _loginPlayerID2Socket.count(playerID))
      return;

    unsigned int socketFD = _loginPlayerID2Socket[playerID];
    _loginPlayerID2Socket.erase(playerID);
    _socket2LoginPlayerID.erase(socketFD);
  }
  
  void PlayerManager::RemoveLoginPlayerWithSocketFD(unsigned int socketFD)
  {
    if(0 == _socket2LoginPlayerID.count(socketFD))
      return;

    unsigned int playerID = _socket2LoginPlayerID[socketFD];
    _loginPlayerID2Socket.erase(playerID);
    _socket2LoginPlayerID.erase(socketFD);
  }

}
