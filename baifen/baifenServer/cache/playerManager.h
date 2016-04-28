#ifndef _PLAYER_MANAGER_H_
#define _PLAYER_MANAGER_H_

#include <string>
#include <map>
#include <baifenSql.h>

namespace face2wind{

  struct PlayerVo
  {
    unsigned int id;
    std::string name;
    std::string password;
  };

  class PlayerManager
  {
  private:
    std::map<std::string,PlayerVo*> _name2voCache;
    std::map<unsigned int,PlayerVo*> _id2voCache;

    std::map<unsigned int,unsigned int> _loginPlayerID2Socket;
    std::map<unsigned int,unsigned int> _socket2LoginPlayerID;
    
    BaifenSql *sql;

  public:
    static PlayerManager *GetInstance();
    PlayerManager(){
      sql = BaifenSql::GetInstance();
    }
    ~PlayerManager(){

    }

    PlayerVo *GetPlayerWithName(const std::string &name);
    PlayerVo *GetPlayerWithID(const unsigned int &id);
    unsigned int CreatePlayer(const std::string &name,const std::string &passwd);
    bool ChangePlayerState(const std::string &name);

    unsigned int GetPlayerIDWithSocketFD(unsigned int socketFD);
    unsigned int GetSocketFDWithPlayerID(unsigned int playerID);
    void SaveLoginPlayerIDAndSocketFD(unsigned int playerID,unsigned int socketFD);
    void RemoveLoginPlayerWithID(unsigned int playerID);
    void RemoveLoginPlayerWithSocketFD(unsigned int socketFD);
  };

}

#endif //_PLAYER_MANAGER_H_
