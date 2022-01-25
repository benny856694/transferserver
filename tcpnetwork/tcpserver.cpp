#include "tcpserver.h"
#include "acceptcallback.h"

TcpServer::TcpServer()
:  accpet_callback_(NULL)
,  handle_(NULL){
	accpet_callback_ = new AcceptCallback();
	handle_ = new aio_handle(ENGINE_KERNEL);
	sstreams.clear();
}

TcpServer::~TcpServer() {
	if(accpet_callback_ != NULL)
	{
		delete accpet_callback_;
		accpet_callback_ = NULL;
	}
	if(handle_ != NULL)
	{
		delete handle_;
		handle_ = NULL;
	}
}
bool TcpServer::Init()
{
	acl::acl_cpp_init();
	bool ret = BindPortServer(9326);
	if(!ret)
	{
		printf("Error\n");
		return false;
	}
	ret = BindPortServer(8439);
	if(!ret)
	{
		printf("Error\n");
		return false;
	}
	printf("Start OK\n");
	return true;
}
void TcpServer::Start()
{
	this->start();
}


void* TcpServer::run(void)
{
	while (true)
	{
		if (handle_->check() == false)
		{
			printf("Stop Server");
			break;
		}
	}
	closealllistenr();
	handle_->check();
	return NULL;
}
bool TcpServer::BindPortServer(int port)
{
	char addr[32];
	sprintf(addr,":%d",port);
	aio_listen_stream* sstream = new aio_listen_stream(handle_);
	if (sstream->open(addr) == false)
	{
		sstream->close();
		handle_->check();
		printf("listen %s error: %s", addr, last_serror());
		return false;
	}
	sstream->add_accept_callback(accpet_callback_);
	sstreams.push_back(sstream);
	return true;
}
void TcpServer::closealllistenr()
{
	std::vector<aio_listen_stream*>::iterator it = sstreams.begin();
	for (; it != sstreams.end(); ++it)
	{
		(*it)->close();
	}
}
