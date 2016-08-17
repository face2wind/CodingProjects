#include <iostream>
#include <string>
#include <iomanip>
#include <elegance.hpp>

using namespace std;
using namespace face2wind;

struct AAA
{
  AAA() : c1(-10), c2(12), s1(-20), s2(24), f1(123.456f), i1(-12345), i2(23456), 
          d1(-12345.6789), ll1(-88888), ll2(99999), str("I love you!") {}

  char c1;
  unsigned char c2;
  short s1;
  unsigned short s2;
  float f1;
  int i1;
  unsigned int i2;
  double d1;
  long long ll1;
  unsigned long long ll2;
  string str;
};
#pragma pack(push, 4)
struct memtest
{
	memtest() : i1(0), s1(0), s2(0) {}
	int i1;
	short s1;
	short s2;
};

struct memtest2
{
	memtest2() : i1(0), s1(0), i2(0), s2(0) {}
	int i1;
	short s1;
	int i2;
	short s2;
	short s4;
};

struct memtest3
{
	memtest3() : i1(0), s1(0), s3(0), i2(0), s2(0) {}
	int i1;
	short s1;
	short s3;
	int i2;
	short s2;
	short s4;
};
#pragma pack(pop)

int main()
{
  AAA a;

  ByteArray by;
  by.WriteInt8(a.c1);
  by.WriteUint8(a.c2);
  by.WriteInt16(a.s1);
  by.WriteUint16(a.s2);
  by.WriteFloat(a.f1);
  by.WriteInt32(a.i1);
  by.WriteUint32(a.i2);
  by.WriteDouble(a.d1);
  by.WriteInt64(a.ll1);
  by.WriteUint64(a.ll2);
  by.WriteString(a.str);

  cout << (int)by.ReadInt8() << endl;
  cout << (int)by.ReadUint8() << endl;
  cout << by.ReadInt16() << endl;
  cout << by.ReadUint16() << endl;
  cout << setprecision(10) << by.ReadFloat() << endl;
  cout << by.ReadInt32() << endl;
  cout << by.ReadUint32() << endl;
  cout << by.ReadDouble() << endl;
  cout << by.ReadInt64() << endl;
  cout << by.ReadUint64() << endl;
  cout << by.ReadString() << endl;

  by.WriteObject(&a, sizeof(AAA));
  AAA *b = (AAA*)by.ReadObject(sizeof(AAA));
  cout << b->d1 << endl;

  int LOOP_TIMES = 10000;
 
  long long pre_time = Timer::GetNowTimeMS();
  for (int i = 0; i < LOOP_TIMES; ++ i)
  {
    AAA a;
    ByteArray by;
    by.WriteInt8(a.c1);
    by.WriteUint8(a.c2);
    by.WriteInt16(a.s1);
    by.WriteUint16(a.s2);
    by.WriteFloat(a.f1);
    by.WriteInt32(a.i1);
    by.WriteUint32(a.i2);
    by.WriteDouble(a.d1);
    by.WriteInt64(a.ll1);
    by.WriteUint64(a.ll2);
    by.WriteString(a.str);
  }
  cout << "bytearray loop time : " << Timer::GetNowTimeMS() - pre_time << " ms" << endl;

  pre_time = Timer::GetNowTimeMS();
  for (int i = 0; i < LOOP_TIMES; ++i)
  {
    AAA a,b;
    a = b;
    //memcpy(&a, &b, sizeof(AAA));
  }
  cout << "memcpy loop time : " << Timer::GetNowTimeMS() - pre_time << " ms" << endl;

  char buff[128];
  memset(buff, 0, sizeof(buff));

  memtest *tt = (memtest*)buff;
  tt->i1 = 2;
  tt->s1 = 3;
  cout << "memtest i1 " << tt->i1 <<", s1 "<<tt->s1<<", s2 "<<tt->s2<< endl;
  memtest2 *tt2 = (memtest2*)buff;
  tt2->i2 = 4;
  cout << "memtest i2 " << tt2->i1 << ", s1 " << tt2->s1 << ", s2 " << tt2->s2 <<", i2 "<<tt2->i2<<", s4 "<<tt2->s4<< endl;
  memtest3 *tt3 = (memtest3*)buff;
  cout << "memtest i3 " << tt3->i1 << ", s1 " << tt3->s1 << ", s2 " << tt3->s2 << ", i2 " << tt3->i2 <<", s3 "<<tt3->s3 << ", s4 " << tt2->s4 << endl;

  return 0;
}
