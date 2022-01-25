

#ifndef TCPNETWORK_NETTIMER_H_
#define TCPNETWORK_NETTIMER_H_
#include "../netlib/inc/acl_cpp/lib_acl.hpp"

using namespace acl;
class NetTimer: public aio_timer_callback{
public:
	NetTimer();
	~NetTimer();
protected:
	virtual void timer_callback(unsigned int id);
	virtual void destroy(void);
};

#endif /* TCPNETWORK_NETTIMER_H_ */
