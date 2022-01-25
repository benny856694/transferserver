

#ifndef TCPNETWORK_TCPSERVER_H_
#define TCPNETWORK_TCPSERVER_H_
#include "../netlib/inc/acl_cpp/lib_acl.hpp"
using namespace acl;

class MyMasterAio;
class AcceptCallback;
class TcpServer : public thread{
public:
	TcpServer();
	~TcpServer();
	bool Init();
	void Start();
protected:
	virtual void* run(void);
private:
	bool BindPortServer(int port);
	void closealllistenr();
private:
	AcceptCallback *accpet_callback_;
	aio_handle *handle_;
	std::vector<aio_listen_stream*> sstreams;
};

#endif /* TCPNETWORK_TCPSERVER_H_ */
