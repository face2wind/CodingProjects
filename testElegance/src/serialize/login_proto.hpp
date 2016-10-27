#pragma once

#include <string>
#include <vector>
#include <memory/serialize/serialize_base.hpp>

using face2wind::SerializeBase;
using face2wind::SerializeDescribe;
using face2wind::ByteArray;

namespace Protocol {

class CSRegisterAccount : public SerializeBase
{
public:
  std::string name;
  std::string passwd;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);

protected:
  virtual SerializeBase * Clone() const { return new CSRegisterAccount(); }
  virtual const std::string GetTypeName() const { return "CSRegisterAccount"; }
};

class CSLogin : public SerializeBase
{
public:
  std::string name;
  std::string passwd;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);

protected:
  virtual SerializeBase * Clone() const { return new CSLogin(); }
  virtual const std::string GetTypeName() const { return "CSLogin"; }
};

class SCLoginResult : public SerializeBase
{
public:
  int result;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);

protected:
  virtual SerializeBase * Clone() const { return new SCLoginResult(); }
  virtual const std::string GetTypeName() const { return "SCLoginResult"; }
};

class TestProto : public SerializeBase
{
public:
  std::vector<std::string> name_list;
  std::vector<CSLogin> login_list;

  virtual void Serialize(ByteArray &collector) const;
  virtual void Unserialize(ByteArray &collector);

protected:
  virtual SerializeBase * Clone() const { return new TestProto(); }
  virtual const std::string GetTypeName() const { return "TestProto"; }
};

class __CSRegisterAccountDescribe__ : public SerializeDescribe
{
public:
  __CSRegisterAccountDescribe__() { name_to_object_map_["CSRegisterAccount"] = this; }
  virtual ~__CSRegisterAccountDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSRegisterAccount(); }
};

class __CSLoginDescribe__ : public SerializeDescribe
{
public:
  __CSLoginDescribe__() { name_to_object_map_["CSLogin"] = this; }
  virtual ~__CSLoginDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new CSLogin(); }
};

class __SCLoginResultDescribe__ : public SerializeDescribe
{
public:
  __SCLoginResultDescribe__() { name_to_object_map_["SCLoginResult"] = this; }
  virtual ~__SCLoginResultDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new SCLoginResult(); }
};

class __TestProtoDescribe__ : public SerializeDescribe
{
public:
  __TestProtoDescribe__() { name_to_object_map_["TestProto"] = this; }
  virtual ~__TestProtoDescribe__() {}

protected:
  virtual SerializeBase * CreateSerialize() const { return new TestProto(); }
};



}

