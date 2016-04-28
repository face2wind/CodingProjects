#ifndef _WRITE_FILE_MODULE_HPP_
#define _WRITE_FILE_MODULE_HPP_

#include <map>
#include <set>
#include <fstream>
#include <boost/thread/mutex.hpp>
#include "network/i_network.hpp"
#include "module/i_module.hpp"
#include "messages.hpp"
#include <vector>
#include "client_network_handler.hpp"

class CommandAnalyser;
struct SCFilePackageBack;

using namespace face2wind;

class WriteFileModule : public face2wind::IModule
{
 public:
  WriteFileModule() : m_next_receive_file_index(0), 
                      m_total_index_count(0), 
                      m_cur_has_transfer_index_count(0), 
                      m_cur_transfer_file_net_id(0),
                      m_cur_transfer_rate(0),
											m_transfering(false)
  {}
  ~WriteFileModule() {}

  virtual int Init();
  virtual int Start();
  virtual int Update();
  virtual int Stop();
  virtual int Release();

	void PrintProcess();
  void TryWritePack(FileSize pack_index);
  void StartReceiveFile(const std::string &file_name, std::vector<FileSplitInfo> file_split_list, std::set<NetworkID> netid_set);
  void AddFileSplitPack(SCFilePackageBack *msg);

 protected:
  std::map<FileSize, SCFilePackageBack*> m_split_file_packs;
  FileSize m_next_receive_file_index;
  FileSize m_total_index_count;
  FileSize m_cur_has_transfer_index_count;
  face2wind::NetworkID m_cur_transfer_file_net_id;
	std::string m_cur_transfer_file_name;
  std::ofstream m_cur_writing_file;
  boost::mutex m_write_file_lock;
  int m_cur_transfer_rate;

  std::vector<FileSplitInfo> m_file_resume_split_list;
  std::set<FileSize> m_file_resume_split_set;
  std::set<NetworkID> m_net_id_set;

	bool m_transfering;
};

#endif
