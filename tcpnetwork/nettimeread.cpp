

#include "nettimeread.h"

NetTimerRead::NetTimerRead(int delay)
: delay_(delay){
}

NetTimerRead::~NetTimerRead(){
}
void NetTimerRead::destroy(){
	delete this;
}

void NetTimerRead::timer_callback(unsigned int id){
	aio_timer_reader::timer_callback(id);
}
