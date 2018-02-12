all: rtlomniv2 

CC = g++
GCCVERSION = $(shell gcc --version | grep ^gcc | sed 's/^.* //g')
#CFLAGS = -std=c++98
CFLAGS  += -Wall -g -O0 -Wno-unused-variable  -Wno-sign-compare

LDFLAGS	= -lm -lliquid -lpthread


rtlomniv2: RFModem.cpp RFModem.h Packet.cpp Packet.h PacketHandler.cpp PacketHandler.h Message.cpp Message.h SubMessage.h SubMessage.cpp SubMessageStatus.h SubMessageStatus.cpp MessageHandler.h MessageHandler.cpp PDMGetState.h PDMGetState.cpp rtlomniv2.cpp
		$(CC) $(CFLAGS) -o rtlomniv2 RFModem.cpp Packet.cpp Message.cpp PacketHandler.cpp MessageHandler.cpp SubMessage.cpp SubMessageStatus.cpp PDMGetState.cpp rtlomniv2.cpp $(LDFLAGS) 


