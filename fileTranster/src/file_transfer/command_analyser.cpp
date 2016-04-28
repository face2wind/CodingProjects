#include "command_analyser.hpp"
#include <iostream>
#include <cstdlib>

CommandAnalyser::CommandAnalyser(int argc, char **argv)
{
  // reset all datas ===============================================================================
  for (int command_type = COMMAND_TYPE_MIN; command_type <= COMMAND_TYPE_MAX; ++command_type)
  {
    m_command_state_list[command_type] = false;
    switch (command_type)
    {
      case COMMAND_TYPE_SET_HOST:
        m_command_param_list[command_type] = "127.0.0.1";
        break;
      case COMMAND_TYPE_SET_PORT:
        m_command_param_list[command_type] = "9527";
        break;
      case COMMAND_TYPE_SET_MULTIDOWNLOAD:
        m_command_param_list[command_type] = "1";
        break;
      default:
        m_command_param_list[command_type] = "";
        break;
    }
  }

  // analyse params ===============================================================================
  CommandType cur_command_type = COMMAND_TYPE_MAX;
  bool cur_command_has_data = false;

  for (int param_index = 0; param_index < argc; ++param_index)
  {
    if (false == cur_command_has_data) // read param type
    {
      std::string param(argv[param_index]);

      // read param that has no value
      if ("-s" == param)
      {
        m_command_state_list[COMMAND_TYPE_IS_SERVER] = true;
        cur_command_type = COMMAND_TYPE_IS_SERVER;
        cur_command_has_data = false;
      }
      else if ("-c" == param)
      {
        m_command_state_list[COMMAND_TYPE_IS_CLIENT] = true;
        cur_command_type = COMMAND_TYPE_IS_CLIENT;
        cur_command_has_data = false;
      }
      else if ("-v" == param)
      {
        m_command_state_list[COMMAND_TYPE_IS_SHOW_VERSION] = true;
        cur_command_type = COMMAND_TYPE_IS_SHOW_VERSION;
        cur_command_has_data = false;
      }
      else if ("-l" == param)
      {
        m_command_state_list[COMMAND_TYPE_LIST_FILE] = true;
        cur_command_type = COMMAND_TYPE_LIST_FILE;
        cur_command_has_data = false;
      }
      else if ("-g" == param)
      {
        m_command_state_list[COMMAND_TYPE_GET_FILE] = true;
        cur_command_type = COMMAND_TYPE_GET_FILE;
        cur_command_has_data = true;
      }
      else if ("-m" == param)
      {
        m_command_state_list[COMMAND_TYPE_SET_MULTIDOWNLOAD] = true;
        cur_command_type = COMMAND_TYPE_SET_MULTIDOWNLOAD;
        cur_command_has_data = true;
      }
      // read param that has value
      else if ("-h" == param)
      {
        m_command_state_list[COMMAND_TYPE_SET_HOST] = true;
        cur_command_type = COMMAND_TYPE_SET_HOST;
        cur_command_has_data = true;
      }
      else if ("-p" == param)
      {
        m_command_state_list[COMMAND_TYPE_SET_PORT] = true;
        cur_command_type = COMMAND_TYPE_SET_PORT;
        cur_command_has_data = true;
      }
    }
    else // read param value
    {
      m_command_param_list[cur_command_type] = std::string(argv[param_index]);
      cur_command_has_data = false;
    }
  }

}

bool CommandAnalyser::CheckAllParams()
{
  // check if has some options conflicting
  
  if (true == m_command_state_list[COMMAND_TYPE_IS_CLIENT] && true == m_command_state_list[COMMAND_TYPE_IS_SERVER])
  {
    std::cout << "error : can not set -s -c at the same time" << std::endl;
    return false;
  }
  if (false == m_command_state_list[COMMAND_TYPE_IS_SHOW_VERSION] && false == m_command_state_list[COMMAND_TYPE_IS_CLIENT] && false == m_command_state_list[COMMAND_TYPE_IS_SERVER])
  {
    std::cout << "error : must set -s or -c to identify whether run as server or client or just -v to show software version" << std::endl;
    return false;
  }
  return true;
}

CommandAnalyser::~CommandAnalyser()
{

}

void CommandAnalyser::ShowHelp()
{
  using std::cout;
  using std::endl;

  cout << "\n"
       << "usage : file_translate -s/-c/-v [-h/-p/-l/-g/-m]\n"
       << "-s run as server\n"
       << "-c run as client\n"
       << "-v show software version\n"
       << "-p set listening port or remote server port\n"
       << "-h [only for client] set server host address\n\n"
       << std::flush;
}

const std::string CommandAnalyser::GetStringParam(CommandType command_type)
{
  return m_command_param_list[command_type];
}

int CommandAnalyser::GetIntParam(CommandType command_type)
{
  return std::atoi(m_command_param_list[command_type].c_str());
}

bool CommandAnalyser::CheckCommandState(CommandType command_type)
{
  return m_command_state_list[command_type];
}
