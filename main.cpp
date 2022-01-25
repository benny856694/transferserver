
#include "tcpnetwork/tcpserver.h"
#include <stdio.h>
#include <unistd.h>
int main()
{
	TcpServer *server = new TcpServer();
	server->Init();
	server->Start();
	while(1)
	{
		usleep(1000*1000);
	}
	return 0;
}
