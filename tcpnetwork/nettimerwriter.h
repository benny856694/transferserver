

#ifndef TCPNETWORK_NETTIMERWRITER_H_
#define TCPNETWORK_NETTIMERWRITER_H_
#include "../netlib/inc/acl_cpp/lib_acl.hpp"

using namespace acl;
class NetTimerWriter : public aio_timer_writer{
public:
	NetTimerWriter(int delay);
	~NetTimerWriter();
	void destroy();
	virtual void timer_callback(unsigned int id);
private:
	int   delay_;
};

#endif /* TCPNETWORK_NETTIMERWRITER_H_ */
