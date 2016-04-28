#ifndef _MYSQL_CONNECTOR_CPP_
#define _MYSQL_CONNECTOR_CPP_

#include <mysqlConnector.h>
#include <string>
#include <iostream>

namespace face2wind {

  int MysqlConnector::Connect(const std::string &host,
			      const std::string &user,
			      const std::string &passwd,
			      const std::string &dbName)
  {
    curHost = host;
    curUser = user;
    curPassword = passwd;
    curDBName = dbName;
    return Connect();
  }

  int MysqlConnector::Connect()
  {
    if(_connected){
      std::cout<<"has connected !"<<std::endl;
      return 3;
    }
    if("" == curHost ||
       "" == curUser ||
       "" == curDBName){
      std::cout<<"host,user or dbName was empty"<<std::endl;
      return 1;
    }
    if(NULL == connection){
      connection = mysql_init(NULL);
      if(NULL == connection){
	std::cout<<"init Error:"<<mysql_error(connection)<<std::endl;
	return 2;
      }
    }
    std::cout<<"connect mysql : "
	     <<curUser<<"@"<<curHost
	     <<",pass = "<<curPassword
	     <<",db = "<<curDBName<<std::endl;
    connection = mysql_real_connect(connection,
				    curHost.c_str(),
				    curUser.c_str(),
				    curPassword.c_str(),
				    curDBName.c_str(),
				    0, NULL, 0);
    if(NULL == connection){
      std::cout<<"connect Error:" << mysql_error(connection)<<std::endl;
      return 3;
    }
    std::cout<<"sql connect success!"<<std::endl;
    _connected = true;
    return 0;
  }

  SqlResult *MysqlConnector::Query(const std::string &command)
  {
    if(!_connected){
      std::cout<<"current sql has not connected!"<<std::endl;
      return NULL;
    }
    if(mysql_query(connection, command.c_str())){
      std::cout<<"Query Error: "<<mysql_error(connection)<<std::endl;
      return NULL;
    }
    SqlResult *result = new SqlResult();
    result->ReadSqlResult(connection);
    return result;
  }

  int SqlResult::ClearOldResult()
  {
    for(std::vector<SqlResultRow*>::iterator it = _result.begin();
	it != _result.end(); ++it)
      delete (*it);
    _result.clear();
    return 0;
  }

  int SqlResult::ReadSqlResult(MYSQL *connection)
  {
    ClearOldResult();

    MYSQL_RES *queryResult = mysql_use_result(connection);
    int resultNum = mysql_field_count(connection);
    if(1 > resultNum)
      return 1;
    int propertyNum = mysql_num_fields(queryResult);
    
    for(int i = 0; i < resultNum; ++i){
      MYSQL_ROW queryResultRow = mysql_fetch_row(queryResult);
      if(NULL == queryResultRow )
	break;
      SqlResultRow *row = new SqlResultRow();
      for(int j = 0; j < propertyNum; ++j){
	//std::cout<<"char*("<<(char*)(queryResultRow[j])<<") ";
	//std::cout<<queryResultRow[j]<<" , ";
	row->PushData(queryResultRow[j]);
      }
      std::cout<<std::endl;
      mysql_free_result(queryResult);
      _result.push_back(row);
    }
    return 0;
  }

  int SqlResultRow::PushData(char *data)
  {
    _row.push_back(new std::string(data));
    return 0;
  }

  int SqlResultRow::ReadInt(unsigned int index)
  {
    if(0 > index || _row.size() < (index+1) )
      return 0;

    return std::stoi(*(_row[index]));
  }
  
  std::string SqlResultRow::ReadString(unsigned int index)
  {
    if(0 > index || _row.size() < (index+1) )
      return 0;
    
    return *(_row[index]);
  }


}

#endif //_MYSQL_CONNECTOR_CPP_
