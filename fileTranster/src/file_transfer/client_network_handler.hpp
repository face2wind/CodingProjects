#ifndef _CLIENT_NETWORK_HANDLER_HPP_
#define _CLIENT_NETWORK_HANDLER_HPP_

#include "network/i_network.hpp"
#include <map>
#include "messages.hpp"
#include <fstream>      // std::ofstream
#include <set>
#include <vector>

class CommandAnalyser;
class WriteFileModule;

using namespace face2wind;

namespace face2wind {
class Network;
}

const static int MULTIPLE_DOWNLOAD_NUM = 8;

struct FileSplitInfo
{
  FileSize start_index;
  FileSize end_index;
    
  bool operator <(const FileSplitInfo &rhs)  const
  {
    FileSize size1 = end_index - start_index;
    FileSize size2 = rhs.end_index - rhs.start_index;
    return size1 < size2;
  }
  bool operator >(const FileSplitInfo &rhs)  const
  {
    FileSize size1 = end_index - start_index;
    FileSize size2 = rhs.end_index - rhs.start_index;
    return size1 > size2;
  }
};

class ClientNetworkHandler : public INetworkHandler
{
 public:
  ClientNetworkHandler(CommandAnalyser *analyser);
  ~ClientNetworkHandler() {}

  virtual void OnConnect(bool is_success, NetworkID network_id, Port local_port, IPAddr remote_ip_addr, Port remote_port);
  virtual void OnAccept(NetworkID network_id, Port listen_port, IPAddr remote_ip_addr, Port remote_port);
  virtual void OnRecv(NetworkID network_id, const char *data, int length);
  virtual void OnDisconnect(NetworkID network_id);

  void Running();

 protected:
  void StartMultipleDownlaod();
  void CalculateMultipleDownlaod();

  void OnFileListBack(NetworkID network_id, const char *data, int length);
  void OnFileMsgBack(NetworkID network_id, const char *data, int length);
  void OnFilePackageBack(NetworkID network_id, const char *data, int length);

  typedef void (ClientNetworkHandler::*HandleFunc)(NetworkID network_id, const char *data, int length);

 private:
  CommandAnalyser *m_analyser;
  face2wind::Network *m_networkMgr;
  std::map<HeadType, HandleFunc> m_handler_map;
  WriteFileModule *m_write_file_module;
  std::vector<FileSplitInfo> m_file_resume_split_list;
  std::set<NetworkID> m_net_id_set;
  std::string m_cur_translate_file_name;
};

#endif
