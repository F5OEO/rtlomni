#ifndef _PACKET_HANDLER
#define _PACKET_HANDLER

#include "RFModem.h"
#include "Packet.h"


class PacketHandler
{

private:
RFModem *modem;

unsigned char Frame[MAX_BYTE_PER_PACKET];
bool Monitoring=true;
bool Transmitting=false;
public:
unsigned char Sequence=0xFF;
bool IsValid=false;
Packet rcvpacket;
Packet txpacket;
Packet txack;

PacketHandler();
PacketHandler(RFModem *current_modem,bool Monitoring_mode=true);
~PacketHandler();
void Init(RFModem *current_modem,bool Monitoring_mode);
int WaitForNextPacket();

int TxAck(int AckSequence);
int TxPacket(Packet *packet_to_tx);
int TxPacketWaitAck(Packet *packet_to_tx,int MaxRetry,bool LastPacket);
};
#endif
