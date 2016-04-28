#ifndef _NETWORK_MESSAGES_HPP_
#define _NETWORK_MESSAGES_HPP_

typedef unsigned short HeadType;
typedef unsigned long long FileSize;

const static int FILE_NAME_LENGTH_MAX = 127;
static const int FILE_LIST_MAX_LENGTH = 50;
static const int SPLIT_FILE_PACK_MAX_SIZE = 40960;

struct BaseMessage
{
  BaseMessage(HeadType type) : head(type) {}
  
  HeadType head;
};

struct FileMsg
{
  char name[FILE_NAME_LENGTH_MAX + 1];
  FileSize size;
};

enum ProtoType
{
  MESSAGE_MIN_TYPE = 10000,
  REQUEST_FILE_LIST = MESSAGE_MIN_TYPE,
  FILE_LIST_BACK,
  REQUEST_GET_FILE_INFO,
  FILE_INFO_BACK,
  REQUEST_GET_FILE_PACKAGE,
  FILE_PACKAGE_BACK,

  MESSAGE_MAX_TYPE,
};

struct CSRequestFileList
{
  CSRequestFileList() : head(ProtoType::REQUEST_FILE_LIST) {}
  HeadType head;
};

struct SCFileListBack
{
  SCFileListBack() : head(ProtoType::FILE_LIST_BACK) {}
  HeadType head;

  int file_num;
  FileMsg file_list[FILE_LIST_MAX_LENGTH + 1];
};

/**
 * request to get file info
 */
struct CSRequestFileInfo
{
  CSRequestFileInfo() : head(ProtoType::REQUEST_GET_FILE_INFO) {}
  HeadType head;

  char file_path[FILE_NAME_LENGTH_MAX + 1];
};

struct SCFileInfoBack
{
  SCFileInfoBack() : head(ProtoType::FILE_INFO_BACK) {}
  HeadType head;

  FileMsg file_msg;
};

/**
 * request to get some part of the file with start_index and end_index
 */
struct CSRequestGetFile
{
  CSRequestGetFile() : head(ProtoType::REQUEST_GET_FILE_PACKAGE) {}
  HeadType head;

  char file_path[FILE_NAME_LENGTH_MAX + 1];
  FileSize start_index;
  FileSize end_index;
};

struct SCFilePackageBack
{
  SCFilePackageBack() : head(ProtoType::FILE_PACKAGE_BACK) {}
  HeadType head;

  FileSize current_pack_index;
  unsigned int current_pack_size;
  char split_file_pack[SPLIT_FILE_PACK_MAX_SIZE];
};

// ===========================================================
struct NoUseJustForMax
{
  NoUseJustForMax() : head(ProtoType::MESSAGE_MAX_TYPE) {}
  
  HeadType head;
};

#endif
