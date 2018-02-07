all: rtlomniv2 

CC = g++
GCCVERSION = $(shell gcc --version | grep ^gcc | sed 's/^.* //g')
#CFLAGS = -std=c++98
CFLAGS  += -Wall -g -O0 -Wno-unused-variable  -Wno-sign-compare

LDFLAGS	= -lm -lliquid -lpthread


rtlomniv2: RFModem.cpp RFModem.h Packet.cpp Packet.h PacketHandler.cpp PacketHandler.h Message.cpp Message.h rtlomniv2.cpp
		$(CC) $(CFLAGS) -o rtlomniv2 RFModem.cpp Packet.cpp Message.cpp PacketHandler.cpp rtlomniv2.cpp $(LDFLAGS) 


