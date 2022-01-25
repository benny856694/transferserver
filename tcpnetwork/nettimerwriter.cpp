

#include "nettimerwriter.h"

NetTimerWriter::NetTimerWriter(int delay)
:delay_(delay){
}
NetTimerWriter::~NetTimerWriter(){
}
void NetTimerWriter::destroy(){
	delete this;
}
void NetTimerWriter::timer_callback(unsigned int id){
	aio_timer_writer::timer_callback(id);
}
