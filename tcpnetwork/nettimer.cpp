

#include "nettimer.h"

NetTimer::NetTimer()
: aio_timer_callback(true){
}
NetTimer::~NetTimer(){

}
void NetTimer::timer_callback(unsigned int id){
}
void NetTimer::destroy(void){
	delete this;
}
