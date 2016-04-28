#ifndef _BAIFEN_SQL_H_
#define _BAIFEN_SQL_H_

#include <mysqlConnector.h>

namespace face2wind{

  class BaifenSql : public face2wind::MysqlConnector
  {
  public:
    BaifenSql(){
      //Connect("localhost","root","12345","Baifen");
      Connect("localhost","root","mqmblgba","Baifen");
    }
    ~BaifenSql(){

    }
    static BaifenSql *GetInstance();
  };

}

#endif //_BAIFEN_SQL_H_
