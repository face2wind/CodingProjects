#include "client_network_handler.hpp"
#include "network/network.hpp"
#include "command_analyser.hpp"
#include "messages.hpp"
#include "write_file_module.hpp"
#include <iostream>
#include <iomanip>
#include <boost/filesystem.hpp>
#include "common/tools.hpp"
#include "module/module_driver.hpp"
#include <algorithm>

ClientNetworkHandler::ClientNetworkHandler(CommandAnalyser *analyser) : m_analyser(analyser), m_networkMgr(face2wind::Network::GetInstance()),
                                                                        m_write_file_module(new WriteFileModule())
{
  m_handler_map[ProtoType::FILE_LIST_BACK] = &ClientNetworkHandler::OnFileListBack;
  m_handler_map[ProtoType::FILE_INFO_BACK] = &ClientNetworkHandler::OnFileMsgBack;
  m_handler_map[ProtoType::FILE_PACKAGE_BACK] = &ClientNetworkHandler::OnFilePackageBack;
}

void ClientNetworkHandler::OnConnect(bool is_success, NetworkID network_id, Port local_port, IPAddr remote_ip_addr, Port remote_port)
{
  //std::cout << "Connect success " << network_id << std::endl;
  if (m_net_id_set.empty()) // multiple socket connection just for downlaoding
  {
    if (m_analyser->CheckCommandState(COMMAND_TYPE_LIST_FILE))
    {
      CSRequestFileList req;
      m_networkMgr->AsyncSendData(network_id, (char *)&req, sizeof(req));
    }
    else if (m_analyser->CheckCommandState(COMMAND_TYPE_GET_FILE))
    {
      CSRequestFileInfo req;
      std::string get_file_str = m_analyser->GetStringParam(COMMAND_TYPE_GET_FILE);
      int len = get_file_str.size() > FILE_NAME_LENGTH_MAX ? FILE_NAME_LENGTH_MAX : get_file_str.size();
      strncpy(req.file_path, get_file_str.c_str(), len);
      req.file_path[len] = 0;
      m_networkMgr->AsyncSendData(network_id, (char *)&req, sizeof(req));
    }
    m_net_id_set.insert(network_id);
  }
  else
  {
    m_net_id_set.insert(network_id);
    if (m_net_id_set.size() == m_file_resume_split_list.size()) // all connetion ready
    {
      this->StartMultipleDownlaod();
    }
  }
}

void ClientNetworkHandler::OnAccept(NetworkID network_id, Port listen_port, IPAddr remote_ip_addr, Port remote_port)
{
  
}

void ClientNetworkHandler::OnRecv(NetworkID network_id, const char *data, int length)
{
  if (length < (int)sizeof(BaseMessage))
    return;

  BaseMessage *req = (BaseMessage*)data;
  if (req->head < ProtoType::MESSAGE_MIN_TYPE || req->head >= ProtoType::MESSAGE_MAX_TYPE)
  {
    std::cout << "proto type [" << req->head << "] illegal, ignore it!" << std::endl;
    return;
  }

  if (m_handler_map.count(req->head) > 0)
  {
    //std::cout << "receive message : " << req->head << ", handl it !" << std::endl;
    (this->*m_handler_map[req->head])(network_id, data, length);
  }
  else
  {
    std::cout << "receive message [" << req->head << "] with no handler" << std::endl;
  }
}

void ClientNetworkHandler::OnDisconnect(NetworkID network_id)
{
  m_net_id_set.erase(network_id);
  //std::cout<<"Disconnect : "<<network_id<<std::endl;
  if (m_net_id_set.empty())
  {
    //std::cout << "All Net Disconnected , Now Stop Game" << std::endl;
    m_networkMgr->Stop();
    ModuleDriver::GetInstance()->Exist();
  }
}

void ClientNetworkHandler::Running()
{
  m_networkMgr->RegistHandler(this);
  m_networkMgr->AsyncConnect(m_analyser->GetStringParam(COMMAND_TYPE_SET_HOST), m_analyser->GetIntParam(COMMAND_TYPE_SET_PORT));
  m_networkMgr->AsyncRun();

  ModuleDriver::GetInstance()->RegisterModule("write_file_module", m_write_file_module);
  ModuleDriver::GetInstance()->Run();
}

void ClientNetworkHandler::StartMultipleDownlaod()
{
  //for (std::set<NetworkID>::iterator it =  m_net_id_set.begin(); it != m_net_id_set.end(); ++ it)
  //m_networkMgr->Disconnect(*it);

  m_write_file_module->StartReceiveFile(m_cur_translate_file_name, m_file_resume_split_list, m_net_id_set);

  CSRequestGetFile cs_getfile;
  strcpy(cs_getfile.file_path, m_cur_translate_file_name.c_str());
  cs_getfile.file_path[FILE_NAME_LENGTH_MAX] = 0;

  std::vector<FileSplitInfo>::iterator file_split_it = m_file_resume_split_list.begin();
  for (std::set<NetworkID>::iterator netid_it =  m_net_id_set.begin(); netid_it != m_net_id_set.end(); ++ netid_it)
  {
    if (file_split_it != m_file_resume_split_list.end())
    {
      //std::cout<<"net id "<<*netid_it<<", request split file "<<file_split_it->start_index<<", "<<file_split_it->end_index<<std::endl;
      cs_getfile.start_index = file_split_it->start_index;
      cs_getfile.end_index = file_split_it->end_index;
      m_networkMgr->AsyncSendData(*netid_it, (char *)&cs_getfile, sizeof(cs_getfile));
      
      ++ file_split_it;
    }
  }
}

void ClientNetworkHandler::CalculateMultipleDownlaod()
{
  if (m_file_resume_split_list.size() <= 0)
    return;

  
  int real_multiple_downlaod_num = MULTIPLE_DOWNLOAD_NUM;
  if (MULTIPLE_DOWNLOAD_NUM < m_file_resume_split_list.size())
  {
    real_multiple_downlaod_num = m_file_resume_split_list.size();
  }
  else
  {
    FileSplitInfo tmp_info;
    FileSplitInfo split_1;
    FileSplitInfo split_2;
    // if ( total size < SPLIT_FILE_PACK_MAX_SIZE ) xxxxxx
    int differ_num = MULTIPLE_DOWNLOAD_NUM - m_file_resume_split_list.size();
    while (differ_num > 0)
    {
      std::sort(m_file_resume_split_list.begin(), m_file_resume_split_list.end(), std::less<FileSplitInfo>());
      tmp_info = *(m_file_resume_split_list.rbegin());
      m_file_resume_split_list.pop_back();

      split_1.start_index = tmp_info.start_index;
      split_1.end_index = (tmp_info.end_index + tmp_info.start_index) / 2;
      split_2.start_index = split_1.end_index + 1;
      split_2.end_index = tmp_info.end_index;
      m_file_resume_split_list.push_back(split_1);
      if (split_2.start_index <= split_2.end_index)
        m_file_resume_split_list.push_back(split_2);

      //std::cout<<"split "<<tmp_info.start_index << ", "<<tmp_info.end_index<<" -> "
      //         <<split_1.start_index<<", "<<split_1.end_index<<"  "<<split_2.start_index<<", "<<split_2.end_index<<std::endl;
      
      -- differ_num;
    }
  }

  std::cout << "start " << real_multiple_downlaod_num << " thread to download ..." << std::endl;
  for (int i = 1; i < real_multiple_downlaod_num; ++i) // aleady has one socket connecting
  {
    m_networkMgr->AsyncConnect(m_analyser->GetStringParam(COMMAND_TYPE_SET_HOST), m_analyser->GetIntParam(COMMAND_TYPE_SET_PORT));
  }
}

void ClientNetworkHandler::OnFileListBack(NetworkID network_id, const char *data, int length)
{
  SCFileListBack *msg = (SCFileListBack*)data;
  std::cout << "file num : " << msg->file_num << std::endl;
  for (int i = 0; i < msg->file_num; ++i)
    std::cout << msg->file_list[i].name <<", "<< face2wind::BytesToSizeStr(msg->file_list[i].size) <<std::endl;
  m_networkMgr->Disconnect(network_id);
}

void ClientNetworkHandler::OnFileMsgBack(NetworkID network_id, const char *data, int length)
{
  using namespace boost::filesystem;

  SCFileInfoBack *msg = (SCFileInfoBack *)data;
  if (msg->file_msg.size <= 0)
  {
    std::cout << "file size 0, cancel download" << std::endl;
    return;
  }

  path file_path(msg->file_msg.name);
  if (exists(file_path))
  {
    if (file_size(file_path) >= msg->file_msg.size)
    {
      std::cout << "file aleady exist, transfer cancle" << std::endl;
      m_networkMgr->Disconnect(network_id);
      return;
    }

    path resume_download_file(msg->file_msg.name + std::string(".rdf"));
    if (!exists(resume_download_file))
    {
      std::cout << "file last transfer not complete, but missing resume download file ["<< resume_download_file.string() <<"], transfer cancle" << std::endl;
      m_networkMgr->Disconnect(network_id);
      return;
    }
		
    // last transfer not complete, continue download
    std::ifstream read_resume_file(resume_download_file.string(), std::ios::in);
    if (!read_resume_file.is_open())
    {
      std::cout << "can not open resume download file[" << resume_download_file.string() << "], transfer cancle" << std::endl;
      m_networkMgr->Disconnect(network_id);
      return;
    }

    bool read_some_error(false);
    m_file_resume_split_list.clear();

    while (!read_resume_file.eof())
    {
      FileSplitInfo info;
      read_resume_file >> info.start_index >> info.end_index;
      if (info.start_index < 0 || info.end_index > msg->file_msg.size || info.start_index >= info.end_index)
      {
        read_some_error = true;
        break;
      }
      m_file_resume_split_list.push_back(info);
    }

    if (read_some_error)
    {
      FileSplitInfo info;
      info.start_index = 0;
      info.end_index = msg->file_msg.size;

      m_file_resume_split_list.clear();
      m_file_resume_split_list.push_back(info);
    }
    read_resume_file.close();
  }
  else // no file , download it from new
  {
    FileSplitInfo info;
    info.start_index = 0;
    FileSize end_index = msg->file_msg.size / SPLIT_FILE_PACK_MAX_SIZE;
    //if (end_index < (msg->file_msg.size * 1.0 / SPLIT_FILE_PACK_MAX_SIZE))
    //  ++ end_index;
    info.end_index = end_index;

    m_file_resume_split_list.clear();
    m_file_resume_split_list.push_back(info);
  }

  m_cur_translate_file_name = msg->file_msg.name;

  this->CalculateMultipleDownlaod();
}

void ClientNetworkHandler::OnFilePackageBack(NetworkID network_id, const char *data, int length)
{
  m_write_file_module->AddFileSplitPack((SCFilePackageBack*)data);
}
