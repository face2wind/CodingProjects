#include "login_proto.hpp"

namespace Protocol {

__CSRegisterAccountDescribe__ for_describe_register_to___csregisteraccountdescribe__;
__CSLoginDescribe__ for_describe_register_to___cslogindescribe__;
__SCLoginResultDescribe__ for_describe_register_to___scloginresultdescribe__;
__TestProtoDescribe__ for_describe_register_to___testprotodescribe__;


void CSRegisterAccount::Serialize(ByteArray &collector) const
{
  collector.WriteString(name);
  collector.WriteString(passwd);
}

void CSRegisterAccount::Unserialize(ByteArray &collector)
{
  name = collector.ReadString();
  passwd = collector.ReadString();
}

void CSLogin::Serialize(ByteArray &collector) const
{
  collector.WriteString(name);
  collector.WriteString(passwd);
}

void CSLogin::Unserialize(ByteArray &collector)
{
  name = collector.ReadString();
  passwd = collector.ReadString();
}

void SCLoginResult::Serialize(ByteArray &collector) const
{
  collector.WriteInt32(result);
}

void SCLoginResult::Unserialize(ByteArray &collector)
{
  result = collector.ReadInt32();
}

void TestProto::Serialize(ByteArray &collector) const
{
  collector.WriteUint16((unsigned short)name_list.size());
  for (auto array_item : name_list)
  {
    collector.WriteString(array_item);
  }
  collector.WriteUint16((unsigned short)login_list.size());
  for (auto array_item : login_list)
  {
    array_item.Serialize(collector);
  }
}

void TestProto::Unserialize(ByteArray &collector)
{
  {
    int array_size = collector.ReadUint16();
    std::string tmp_attr_value;
    for (int index = 0; index < array_size; ++ index)
    {
      collector.WriteString(tmp_attr_value);
      name_list.push_back(tmp_attr_value);
    }
  }
  {
    int array_size = collector.ReadUint16();
    CSLogin tmp_attr_value;
    for (int index = 0; index < array_size; ++ index)
    {
      tmp_attr_value.Unserialize(collector);
      login_list.push_back(tmp_attr_value);
    }
  }
}

}

