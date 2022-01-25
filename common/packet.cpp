

#include "packet.h"
#include "pkg.h"
#include "networkmsg.h"


int PackHeartBeat(int system_type, char data[8]) {
  int type = 0;

  type = CreateType(system_type, 1, 14, MESSAGE_ID_HEART);

  struct Packet pkg;

  InitPkg(&pkg, (unsigned char*)data, 8, little_endian);

  PutInt(&pkg, type);
  PutInt(&pkg, 0);

  PkgEnd(&pkg);

  return 8;
}

int PackGetCameraID(int system_type, char data[8]) {
  int type = 0;

  type = CreateType(system_type, 1, 14, MESSAGE_ID_REQ_CAMERAID);

  struct Packet pkg;

  InitPkg(&pkg, (unsigned char*)data, 8, little_endian);

  PutInt(&pkg, type);
  PutInt(&pkg, 0);

  PkgEnd(&pkg);

  return 8;
}
