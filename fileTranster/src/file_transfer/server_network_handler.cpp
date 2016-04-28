#include "server_network_handler.hpp"
#include "network/network.hpp"
#include "command_analyser.hpp"
#include "messages.hpp"
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string.h>
#include <fstream> 
#include "common/tools.hpp"
#include "module/module_driver.hpp"

ServerNetworkHandler::ServerNetworkHandler(CommandAnalyser *analyser) : m_analyser(analyser), m_networkMgr(face2wind::Network::GetInstance())
{
  m_handler_map[ProtoType::REQUEST_FILE_LIST] = &ServerNetworkHandler::OnRequestFileListReq;
  m_handler_map[ProtoType::REQUEST_GET_FILE_INFO] = &ServerNetworkHandler::OnRequestGetFileInfoReq;
  m_handler_map[ProtoType::REQUEST_GET_FILE_PACKAGE] = &ServerNetworkHandler::OnRequestGetFileReq;
}

void ServerNetworkHandler::OnConnect(bool is_success, NetworkID network_id, Port local_port, IPAddr remote_ip_addr, Port remote_port)
{

}

void ServerNetworkHandler::OnAccept(NetworkID network_id, Port listen_port, IPAddr remote_ip_addr, Port remote_port)
{

}

void ServerNetworkHandler::OnRecv(NetworkID network_id, const char *data, int length)
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
    (this->*m_handler_map[req->head])(network_id, data, length);
  }
  else
  {
    std::cout << "receive message [" << req->head << "] with no handler" << std::endl;
  }
}

void ServerNetworkHandler::OnDisconnect(NetworkID network_id)
{
  //m_networkMgr->Stop();
}

void ServerNetworkHandler::Running()
{
  face2wind::Network *networkMgr = face2wind::Network::GetInstance();
  networkMgr->RegistHandler(this);
  networkMgr->AsyncListen(m_analyser->GetIntParam(COMMAND_TYPE_SET_PORT));
	networkMgr->AsyncRun();

	ModuleDriver::GetInstance()->Run();
}

void ServerNetworkHandler::OnRequestFileListReq(NetworkID network_id, const char *data, int length)
{
  using namespace boost::filesystem;

  SCFileListBack send_msg;
  send_msg.file_num = 0;

  path p(".");
  try
  {
    if (exists(p))    // does p actually exist?
    {
      if (is_directory(p))      // is p a directory?
      {
        typedef std::vector<path> vec;             // store paths,
        vec v;                                // so we can sort them later
        copy(directory_iterator(p), directory_iterator(), back_inserter(v));
        sort(v.begin(), v.end());             // sort, since directory iteration
        for (vec::const_iterator it(v.begin()); it != v.end(); ++it)
        {
          if (is_regular_file(*it))
          {
            int name_real_len = it->string().size();
            int name_len = name_real_len > FILE_NAME_LENGTH_MAX ? FILE_NAME_LENGTH_MAX : name_real_len;
            strncpy(send_msg.file_list[send_msg.file_num].name, it->string().c_str(), name_len);
            send_msg.file_list[send_msg.file_num].name[name_len] = 0;
            send_msg.file_list[send_msg.file_num].size = (unsigned int)file_size(*it);
            //std::cout << "   " << *it << ", " << send_msg.file_list[send_msg.file_num].size << '\n';
            ++send_msg.file_num;

            if (send_msg.file_num > FILE_LIST_MAX_LENGTH)
              break;
          }
        }
      }
      else
        std::cout << p << " exists, but is neither a regular file nor a directory\n";
    }
    else
      std::cout << p << " does not exist\n";
  }
  catch (const boost::filesystem::filesystem_error& ex)
  {
    std::cout << ex.what() << std::endl;
  }

  int send_len = sizeof(send_msg) - (FILE_LIST_MAX_LENGTH - send_msg.file_num)*sizeof(FileMsg);
  m_networkMgr->AsyncSendData(network_id, (char *)&send_msg, send_len);
}

void ServerNetworkHandler::OnRequestGetFileInfoReq(NetworkID network_id, const char *data, int length)
{
  CSRequestFileInfo *msg = (CSRequestFileInfo*)data;

  SCFileInfoBack file_msg;
  file_msg.file_msg.size = 0;
  file_msg.file_msg.name[0] = 0;

  using namespace boost::filesystem;
  path file_path(msg->file_path);
  try
  {
    if (exists(file_path) && is_regular_file(file_path))
    {
      strcpy(file_msg.file_msg.name, file_path.string().c_str());
      file_msg.file_msg.size = (FileSize)file_size(file_path);
    }
  }
  catch (const boost::filesystem::filesystem_error& ex)
  {
    std::cout << ex.what() << std::endl;
  }

  std::cout << "receive file info request : " << file_msg.file_msg.name << ", size " << face2wind::BytesToSizeStr(file_msg.file_msg.size) << std::endl;
  m_networkMgr->AsyncSendData(network_id, (char *)&file_msg, sizeof(file_msg));
}

void ServerNetworkHandler::OnRequestGetFileReq(NetworkID network_id, const char *data, int length)
{
  CSRequestGetFile *msg = (CSRequestGetFile*)data;
  if (msg->start_index > msg->end_index)
    return;

  FileSize transfer_file_size(0);
  FileSize real_end_index(0);
  using namespace boost::filesystem;
  path file_path(msg->file_path);
  try
  {
    if (exists(file_path) && is_regular_file(file_path))
    {
      transfer_file_size = (FileSize)file_size(file_path);
      real_end_index = transfer_file_size / SPLIT_FILE_PACK_MAX_SIZE;
      if (real_end_index < (transfer_file_size * 1.0 / SPLIT_FILE_PACK_MAX_SIZE))
        ++real_end_index;
    }
  }
  catch (const boost::filesystem::filesystem_error& ex)
  {
    std::cout << ex.what() << std::endl;
  }

  std::cout << "Client Request File : " << msg->file_path << "  (" << msg->start_index << ", " << msg->end_index<< ")" << std::endl;
  
  if (transfer_file_size > 0 && msg->start_index >= 0 && msg->end_index <= real_end_index) // file not empty , start transfert
  {
    std::ifstream read_file(msg->file_path, std::ios::in | std::ios::binary);


    SCFilePackageBack file_pack;
    file_pack.current_pack_index = msg->start_index;
    file_pack.current_pack_size = 0;
    read_file.seekg(msg->start_index * SPLIT_FILE_PACK_MAX_SIZE);

    while (file_pack.current_pack_index <= msg->end_index && read_file.is_open() && !read_file.eof())
    {
      read_file.read(file_pack.split_file_pack, SPLIT_FILE_PACK_MAX_SIZE);
      file_pack.current_pack_size = (unsigned int)read_file.gcount();

      //std::cout << "pack_size " << file_pack.current_pack_size << ", index " << file_pack.current_pack_index << std::endl;
      int send_length = sizeof(file_pack) - (SPLIT_FILE_PACK_MAX_SIZE - file_pack.current_pack_size);
      m_networkMgr->AsyncSendData(network_id, (char *)&file_pack, send_length);

			if (file_pack.current_pack_index == msg->end_index)
				std::cout << "Transfer File Complete !  (" << msg->start_index << ", " << msg->end_index << ")" << std::endl;
      ++file_pack.current_pack_index;
      //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }

    //std::cout << "Transfer File Complete !  (" << msg->start_index << ", " << msg->end_index << ")" << std::endl;
    read_file.close();
  }
  // message handle end
}
