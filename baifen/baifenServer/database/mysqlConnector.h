#ifndef _MYSQL_CONNECTOR_H_
#define _MYSQL_CONNECTOR_H_

#include <string>
#include <vector>
#include <mysql/mysql.h>

namespace face2wind {

  class SqlResultRow
  {
    std::vector<std::string*> _row;

  public:
    SqlResultRow(){}
    ~SqlResultRow(){
      for(std::vector<std::string*>::iterator it = _row.begin();
	  it != _row.end(); ++it)
	delete (*it);
    }

    int PushData(char *);
    int ReadInt(unsigned int index);
    std::string ReadString(unsigned int index);
  };

  class SqlResult
  {
    std::vector<SqlResultRow*> _result;

  private:
    int ClearOldResult();
    
  public:
    SqlResult(){
    }
    ~SqlResult(){
      ClearOldResult();
    }

    int ReadSqlResult(MYSQL *connection);
    SqlResultRow *GetRow(unsigned int index){
      if(0 > index || _result.size() < (index+1) )
	return NULL;

      return _result[index];
    }
    unsigned int Size(){
      return _result.size();
    }
  };

  class MysqlConnector
  {
  private:
    std::string curHost;
    std::string curUser;
    std::string curPassword;
    std::string curDBName;

    MYSQL *connection;
    bool _connected;

  public:
    MysqlConnector(){
      _connected = false;
    }
    ~MysqlConnector(){}

    int Connect(const std::string &host,
		const std::string &user,
		const std::string &passwd,
		const std::string &dbName);
    int Connect();
    SqlResult *Query(const std::string &command);
  };

}

#endif //_MYSQL_CONNECTOR_H_
