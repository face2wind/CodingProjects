#include <iostream>
#include <string>
#include <iomanip>
#include <elegance.hpp>

using namespace std;
using namespace face2wind;

struct AAA
{
  AAA() : c1(-10), c2(12), s1(-20), s2(24), i1(-12345), i2(23456), f1(123.456f),
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

  return 0;
}
