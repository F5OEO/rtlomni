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
bool Monitoring=true;
bool Transmitting=false;
public:

bool IsValid=false;
Packet rcvpacket;
Packet txpacket;

PacketHandler(RFModem *current_modem,bool Monitoring_mode=true);
~PacketHandler();
int WaitForNextPacket();

int TxPacket();
int TxPacketWaitAck(int MaxRetry);
};
#endif
