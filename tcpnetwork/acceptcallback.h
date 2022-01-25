

#ifndef TCPNETWORK_ACCEPTCALLBACK_H_
#define TCPNETWORK_ACCEPTCALLBACK_H_
#include "../netlib/inc/acl_cpp/lib_acl.hpp"
#include "netdef.h"
#include <list>
#include <vector>
#include <string>
#include <pthread.h>
using namespace acl;
using namespace std;
class AcceptCallback : public aio_accept_callback{
public:
	AcceptCallback();
	~AcceptCallback();
	virtual bool accept_callback(aio_socket_stream* client);
	bool UpdateClientInfo(const ClientStruct client);
	void DeleteClientInfo(const ClientStruct client);
	bool FindClientInfo(const ClientStruct client,vector<ClientStruct> &clint);
	int GetCameraList(vector<ClientStruct> &clint,int offset,int count,char *id,char *addrid,char *addrname,char *custom);
	int GetSize(const ClientStruct client);
	void DisConnNoHeat();
	bool IsExistSn(const char *sn);
private:
	bool extractclient(const char *client,const char *server,ClientStruct &cs);
	void GetSnsByFile(const char *file,vector<std::string> &vects);
private:
	list<ClientStruct *> camera_clients_;
	list<ClientStruct *> windows_clients_;
	vector<std::string> need_collects_sn_;
	pthread_mutex_t camera_mutex_;
	pthread_mutex_t windows_mutex_;
};

#endif /* TCPNETWORK_ACCEPTCALLBACK_H_ */
