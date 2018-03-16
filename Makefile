all: rtlomniv2 

CC = g++
GCCVERSION = $(shell gcc --version | grep ^gcc | sed 's/^.* //g')
#CFLAGS = -std=c++98
CFLAGS  += -Wall -g -O0 -Wno-unused-variable  -Wno-sign-compare

LDFLAGS	= -lm -lliquid -lpthread


rtlomniv2: RFModem.cpp RFModem.h Packet.cpp Packet.h PacketHandler.cpp PacketHandler.h Message.cpp Message.h SubMessage.h SubMessage.cpp SubMessageStatus.h SubMessageStatus.cpp MessageHandler.h MessageHandler.cpp PDMGetState.h Nonce.h Nonce.cpp PDMGetState.cpp rtlomniv2.cpp SubMessageSeed.h SubMessageSeed.cpp PODPairing.h PODPairing.cpp PDMPairing.h PDMPairing.cpp PDMVerifyPairing.h PDMVerifyPairing.cpp PDMCancelTime.h PDMCancelTime.cpp PDMBolus.h PDMBolus.cpp PDMBasal.h PDMBasal.cpp ./librpitx/src/librpitx.a
		$(CC) $(CFLAGS) -o rtlomniv2 RFModem.cpp Packet.cpp Message.cpp PacketHandler.cpp MessageHandler.cpp SubMessage.cpp SubMessageStatus.cpp PDMGetState.cpp Nonce.cpp SubMessageSeed.cpp PODPairing.cpp PDMPairing.cpp PDMVerifyPairing.cpp PDMCancelTime.cpp PDMBolus.cpp PDMBasal.cpp rtlomniv2.cpp ./librpitx/src/librpitx.a $(LDFLAGS) 


