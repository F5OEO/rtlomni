
#ifndef _MESSAGE
#define _MESSAGE

#include "Packet.h"
#define MAX_BYTE_MSG_BODY 32000
#define MAX_PACKET_BY_MESSAGE 10
class Message
{

private:
unsigned char CompleteRawMessage[MAX_BYTE_MSG_BODY];
unsigned int RawLen=0;
public:
unsigned Source=0;
unsigned int ID2=0;
unsigned int Sequence=0;
unsigned int TargetLen=0;
unsigned int MessageLen=0;
unsigned int crc16=0;
unsigned char Body[MAX_BYTE_MSG_BODY];
Packet packet_list[MAX_PACKET_BY_MESSAGE];
int packet_list_len=0;
bool IsValid=false;
int SetMessageFromPacket(Packet *packet);
Message();
~Message();
unsigned int computecrc16(unsigned char *data,int len);
void PrintState();

int SetFirst(Packet *packet);
int SetCon(Packet *packet);
int Reset();

int PacketizeMessage(unsigned int ID1,unsigned int Sequence);
int AddToBody(unsigned char* SubMessage,unsigned int SubMessageLen);
};
#endif
