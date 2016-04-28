#include "write_file_module.hpp"
#include "messages.hpp"
#include <iostream>
#include "common/tools.hpp"
#include "network/network.hpp"

int WriteFileModule::Init()
{
  return 0;
}

int WriteFileModule::Start()
{
  return 0;
}

int WriteFileModule::Update()
{
	if (m_transfering)
		this->PrintProcess();

	return 0;
	if (!m_transfering)
		return 0;

	m_write_file_lock.lock();
	for (std::map<FileSize, SCFilePackageBack*>::iterator it = m_split_file_packs.begin(); it != m_split_file_packs.end(); ++it)
		this->TryWritePack(it->first);
	m_split_file_packs.clear();
	m_write_file_lock.unlock();
	/*
  for (std::vector<FileSplitInfo>::iterator file_split_it = m_file_resume_split_list.begin(); file_split_it != m_file_resume_split_list.end(); ++file_split_it)
  {
    while (m_split_file_packs.count(file_split_it->start_index) > 0)
    {
      this->TryWritePack(file_split_it->start_index);
      ++(file_split_it->start_index);
    }
  } */
  
  //if (m_cur_has_transfer_index_count >= m_total_index_count)
	if (m_file_resume_split_set.empty())
  {
    if (m_cur_writing_file.is_open())
      m_cur_writing_file.close();
    
    std::cout << "transfer complete !" << std::endl;
    for (std::set<NetworkID>::iterator netid_it = m_net_id_set.begin(); netid_it != m_net_id_set.end(); ++netid_it)
    {
      std::cout << "disconnect the net :" << *netid_it << std::endl;
      face2wind::Network::GetInstance()->Disconnect(*netid_it);
    }
		m_transfering = false;
  }

  return 0;
}

void WriteFileModule::PrintProcess()
{
	auto rate = (m_cur_has_transfer_index_count * 1.0) / m_total_index_count;
	int cur_rate = int(rate * 100);
	//if (m_cur_transfer_rate != cur_rate)
	{
		//if (cur_rate > 1)
		{
			std::cout << "\b\b\b\b\b";
			if (cur_rate > 10) // delete one more char
				std::cout << "\b";
		}

		if (0 != cur_rate && cur_rate % 2 == 0)
			std::cout << "#";
		std::cout << " [" << cur_rate << "%]" << std::flush;
		m_cur_transfer_rate = cur_rate;
	}
}

void WriteFileModule::TryWritePack(FileSize pack_index)
{
  SCFilePackageBack* msg = m_split_file_packs[pack_index];
  
  ++m_cur_has_transfer_index_count;
  
  auto rate = (100 * m_cur_has_transfer_index_count) / m_total_index_count;
  double cur_rate = double(FileSize(rate * 100 + 0.5) / 100.0);
  if (m_cur_transfer_rate != cur_rate)
  {
    std::cout << "#" << std::flush;
    //std::cout << "rate " << m_cur_transfer_rate << "%" << std::endl;
    m_cur_transfer_rate = cur_rate;
  }
  if (m_cur_writing_file.is_open())
  {
		m_cur_writing_file.seekp(pack_index * SPLIT_FILE_PACK_MAX_SIZE);
    m_cur_writing_file.write(msg->split_file_pack, msg->current_pack_size);
  }
  
	m_file_resume_split_set.erase(pack_index);
  //m_split_file_packs.erase(pack_index);
  delete msg;
}

int WriteFileModule::Stop()
{
  return 0;
}

int WriteFileModule::Release()
{
  return 0;
}

void WriteFileModule::StartReceiveFile(const std::string &file_name, std::vector<FileSplitInfo> file_split_list, std::set<NetworkID> netid_set)
{
  m_file_resume_split_list = file_split_list;
  for (std::vector<FileSplitInfo>::iterator file_split_it = file_split_list.begin(); file_split_it != file_split_list.end(); ++file_split_it)
  {
    for (FileSize index = file_split_it->start_index; index <= file_split_it->end_index; ++index)
      m_file_resume_split_set.insert(index);
  }
  m_net_id_set = netid_set;
  for (std::vector<FileSplitInfo>::iterator it = file_split_list.begin(); it != file_split_list.end(); ++it)
    m_total_index_count += (it->end_index - it->start_index + 1);
  m_cur_has_transfer_index_count = 0;
  // m_cur_transfer_file_net_id = net_id;

  std::cout << "start transfer file : " << file_name << ", size : " << face2wind::BytesToSizeStr(m_total_index_count * SPLIT_FILE_PACK_MAX_SIZE) << std::endl;
	m_cur_transfer_file_name = file_name;
  m_cur_writing_file.open(file_name, std::ios::out | std::ios::binary | std::ios::trunc);
  if (!m_cur_writing_file.is_open())
  {
    std::cout << m_cur_writing_file.goodbit << std::endl;
    //face2wind::Network::GetInstance()->Disconnect(m_cur_transfer_file_net_id);
  }
	std::cout << " [0%]" << std::flush;
	m_transfering = true;
}

void WriteFileModule::AddFileSplitPack(SCFilePackageBack *msg)
{
	m_write_file_lock.lock();
	//std::cout << "handle index " << msg->current_pack_index << std::flush;
	++m_cur_has_transfer_index_count;
	m_file_resume_split_set.erase(msg->current_pack_index);
	if (m_cur_writing_file.is_open())
	{
		m_cur_writing_file.seekp(msg->current_pack_index * SPLIT_FILE_PACK_MAX_SIZE);
		m_cur_writing_file.write(msg->split_file_pack, msg->current_pack_size);
	}
	m_write_file_lock.unlock();

	if (m_file_resume_split_set.empty())
	{
		this->PrintProcess();

		if (m_cur_writing_file.is_open())
			m_cur_writing_file.close();

		std::cout << "\ntransfer complete !" << std::endl;
		for (std::set<NetworkID>::iterator netid_it = m_net_id_set.begin(); netid_it != m_net_id_set.end(); ++netid_it)
		{
			face2wind::Network::GetInstance()->Disconnect(*netid_it);
		}

		m_transfering = false;
	}

/*
    m_write_file_lock.lock();
    //std::cout << "WriteFileModule::AddFileSplitPack index=" << msg->current_pack_index << std::endl;
    SCFilePackageBack *msg_copy = new SCFilePackageBack();
    // *msg_copy = *msg;
    msg_copy->current_pack_index = msg->current_pack_index;
    msg_copy->current_pack_size = msg->current_pack_size;
    memcpy(msg_copy->split_file_pack, msg->split_file_pack, msg->current_pack_size);

    m_split_file_packs[msg->current_pack_index] = msg_copy;
    m_write_file_lock.unlock();
		*/
}
