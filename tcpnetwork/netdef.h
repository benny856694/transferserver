

#ifndef TCPNETWORK_NETDEF_H_
#define TCPNETWORK_NETDEF_H_
typedef enum
{
	CLINET_TYPE_NULL = 0,
	CLINET_TYPE_CAMERA = 1,
	CLINET_TYPE_CLIENT = 2
}ClientType;

class DataIoCallback;
struct ClientStruct
{
	ClientType type;
	char clientip[32];
	int clientport;
	char serverip[32];
	int serverport;
	DataIoCallback *callback;
	char cameraid[32];
	char addr_id[32];
	char addr_name[96];
	char resv[128];
};
#endif /* TCPNETWORK_NETDEF_H_ */
