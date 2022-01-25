

#ifndef TCPNETWORK_NETTIMEREAD_H_
#define TCPNETWORK_NETTIMEREAD_H_
#include "../netlib/inc/acl_cpp/lib_acl.hpp"

using namespace acl;
class NetTimerRead : public aio_timer_reader{
public:
	NetTimerRead(int delay);
	~NetTimerRead();
	void destroy();
	virtual void timer_callback(unsigned int id);
private:
	int  delay_;
};

#endif /* TCPNETWORK_NETTIMEREAD_H_ */
