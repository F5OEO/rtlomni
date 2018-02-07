
#ifndef _MESSAGE
#define _MESSAGE

#include "Packet.h"
#define MAX_BYTE_MSG_BODY 32000

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
 unsigned char HeaderData[6]; //Needed by CRC16
unsigned char Body[MAX_BYTE_MSG_BODY];

bool IsValid=false;
int SetMessageFromPacket(Packet *packet);
Message();
~Message();
unsigned int computecrc16(unsigned char *data,int len);
void PrintState();

int SetFirst(Packet *packet);
int SetCon(Packet *packet);
int Reset();
};
#endif
