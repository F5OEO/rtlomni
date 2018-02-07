#ifndef _PACKET_HANDLER
#define _PACKET_HANDLER

#include "RFModem.h"
#include "Packet.h"

class PacketHandler
{

private:
RFModem *modem;
unsigned char Sequence=0xFF;
unsigned char Frame[MAX_BYTE_PER_PACKET];
public:

bool IsValid=false;
Packet packet;

PacketHandler(RFModem *current_modem);
~PacketHandler();
int WaitForNextPacket();
};
#endif
