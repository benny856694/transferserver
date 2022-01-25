#ifndef PKG_H
#define PKG_H

enum Endian
{
    big_endian = 0,
    little_endian
};

struct Packet
{
    unsigned char* buffer;
    int buffSize;

    //封包时，返回的长度为填入数据包长度
    //解包时，返回已解数据包长度
    int pkgSize;

    enum Endian pkgEndian;
};


#ifdef __cplusplus
extern "C" {
#endif


int CreateType(int sysType,int majorProtocol,int minorProtocol,int msgType);
void GetType(int type,int *sysType,int *majorProtocol,int *minorProtocol,int *msgType);

void InitPkg(struct Packet* pkg, unsigned char* buffer, int buffSize, enum Endian endian);

//此函数用来填充TLV，的L长度
//只能在填包时用
void PkgEnd(struct Packet* pkg);

//封包时，返回的长度为填入数据包长度
//解包时，返回已解数据包长度
int PkgLen(struct Packet* pkg);

void PutInt(struct Packet* pkg, int value);
void PutUInt(struct Packet* pkg, unsigned int value);

void PutLong(struct Packet* pkg, long value);
void PutULong(struct Packet* pkg, unsigned long value);

void PutShort(struct Packet* pkg, short value);
void PutUShort(struct Packet* pkg, unsigned short value);

void PutChar(struct Packet* pkg, char value);
void PutUChar(struct Packet* pkg, unsigned char value);

void PutFloat(struct Packet* pkg, float value);

void PutString(struct Packet* pkg, char* str, int strLen);
void PutBytes(struct Packet* pkg, unsigned char* str, int strLen);


void GetInt(struct Packet* pkg, int* value);
void GetUInt(struct Packet* pkg, unsigned int* value);

void GetLong(struct Packet* pkg, long* value);
void GetULong(struct Packet* pkg, unsigned long* value);

void GetShort(struct Packet* pkg, short* value);
void GetUShort(struct Packet* pkg, unsigned short* value);

void GetChar(struct Packet* pkg, char* value);
void GetUChar(struct Packet* pkg, unsigned char* value);

void GetFloat(struct Packet* pkg, float* value);

void GetString(struct Packet* pkg, char* str, int strLen);
void GetBytes(struct Packet* pkg, unsigned char* str, int strLen);

unsigned char* Current(struct Packet* pkg);

// 从当前位置跳过一段区域，size大于0时为向前跳跃，小于0则向后跳跃。返回值为bool型。
int Seek(struct Packet* pkg, int size);

#ifdef __cplusplus
}
#endif

#endif //

