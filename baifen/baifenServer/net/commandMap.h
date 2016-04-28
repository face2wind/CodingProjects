#ifndef _COMMAND_MAP_HPP_
#define _COMMAND_MAP_HPP_

#include <map>
#include <socketMessage.h>

#include <c2s/cs100/CS10000.h>
#include <c2s/cs100/CS10001.h>
#include <c2s/cs100/CS10002.h>
#include <c2s/cs100/CS10003.h>
#include <c2s/cs101/CS10100.h>
#include <c2s/cs101/CS10101.h>
#include <c2s/cs101/CS10102.h>
#include <c2s/cs101/CS10103.h>
#include <c2s/cs102/CS10203.h>

#include <s2c/sc100/SC10000.h>
#include <s2c/sc100/SC10001.h>
#include <s2c/sc100/SC10002.h>
#include <s2c/sc100/SC10003.h>
#include <s2c/sc101/SC10100.h>
#include <s2c/sc101/SC10101.h>
#include <s2c/sc101/SC10102.h>
#include <s2c/sc101/SC10103.h>
#include <s2c/sc101/SC10104.h>
#include <s2c/sc102/SC10200.h>
#include <s2c/sc102/SC10201.h>
#include <s2c/sc102/SC10202.h>
#include <s2c/sc102/SC10203.h>

namespace face2wind {

  class CommandMap
  {
    std::map<short, SocketMessage*> csMessages;
    std::map<short, SocketMessage*> scMessages;

  public:
    CommandMap(){
      csMessages[10000] = new CS10000();
      csMessages[10001] = new CS10001();
      csMessages[10002] = new CS10002();
      csMessages[10003] = new CS10003();
      csMessages[10100] = new CS10100();
      csMessages[10101] = new CS10101();
      csMessages[10102] = new CS10102();
      csMessages[10103] = new CS10103();
      csMessages[10203] = new CS10203();

      scMessages[10000] = new SC10000();
      scMessages[10001] = new SC10001();
      scMessages[10002] = new SC10002();
      scMessages[10003] = new SC10003();
      scMessages[10100] = new SC10100();
      scMessages[10101] = new SC10101();
      scMessages[10102] = new SC10102();
      scMessages[10103] = new SC10103();
      scMessages[10104] = new SC10104();
      scMessages[10200] = new SC10200();
      scMessages[10201] = new SC10201();
      scMessages[10202] = new SC10202();
      scMessages[10203] = new SC10203();
    }
    ~CommandMap(){
      csMessages.clear();
      scMessages.clear();
    }

    static CommandMap &GetInstance();

    SocketMessage *GetCSMsgObject(short cmd){
      if(0 < csMessages.count(cmd))
	return csMessages[cmd];
      else
	return NULL;
    }

    SocketMessage *GetSCMsgObject(short cmd){
      if(0 < scMessages.count(cmd))
	return scMessages[cmd];
      else
	return NULL;
    }
  };
}

#endif // _COMMAND_MAP_HPP_