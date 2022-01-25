COMPILER = g++
LINKER	= gcc
INCLUDE = 
CFLAGS = -g

APPNAME = TransferServer

LIBS += -L$(ROOT)netlib/lib/
LIBS += -lfiber_cpp -l_protocol -lacl_cpp -l_acl
LIBS += -pthread
ROOT = 

SRC =     $(ROOT)common/pkg.c \
					$(ROOT)utils/unix_time.c \


	
SRC_CPP =  	$(ROOT)common/packet.cpp \
			    $(ROOT)tcpnetwork/nettimer.cpp \
			    $(ROOT)tcpnetwork/nettimeread.cpp \
			    $(ROOT)tcpnetwork/nettimerwriter.cpp \
			    $(ROOT)tcpnetwork/dataiocallback.cpp \
			    $(ROOT)tcpnetwork/acceptcallback.cpp \
			    $(ROOT)tcpnetwork/tcpserver.cpp \
 			    $(ROOT)main.cpp
OBJ = $(patsubst %.c,%.c.o,$(SRC))
OBJ += $(patsubst %.cpp,%.cpp.o,$(SRC_CPP))




all: $(APPNAME)
### Rules
$(APPNAME): $(OBJ)
	g++ -o $@ $^ $(LIBS)

%.c.o : %.c
	$(LINKER) $(CFLAGS) $(INCLUDE) -c $< -o $@
	
%.cpp.o : %.cpp
	$(COMPILER) -std=c++11 $(CFLAGS) $(INCLUDE) -c $< -o $@
	
clean :
	rm -rf $(OBJ)
	rm -rf $(APPNAME)

