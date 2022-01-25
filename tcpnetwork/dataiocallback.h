

#ifndef TCPNETWORK_DATAIOCALLBACK_H_
#define TCPNETWORK_DATAIOCALLBACK_H_
#include "../netlib/inc/acl_cpp/lib_acl.hpp"
#include "netdef.h"
#include <time.h>
using namespace acl;
class AcceptCallback;
class DataIoCallback : public aio_callback{
public:
	DataIoCallback(aio_socket_stream* client,AcceptCallback *mast,ClientType type);
	~DataIoCallback();
	virtual bool read_callback(char* data, int len);
	virtual bool write_callback();
	virtual void close_callback();
	virtual bool timeout_callback();
	aio_socket_stream* GetPeer();
private:
	bool extractclientip(const char *client,ClientStruct &cs);
	void sendtodata(unsigned char *data,int datalen);
	void SaveFileToFlash(struct timeval* cap_time,char *person_id, int type,int sequence,char *img,int img_len);
private:
	aio_socket_stream* client_;
	AcceptCallback *mast_;
	char id_[32];
	ClientType type_;
	int pkg_type_;
	int pkg_len_;
	int new_pkg_;
};

#endif /* TCPNETWORK_DATAIOCALLBACK_H_ */
