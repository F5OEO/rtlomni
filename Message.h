#include "Packet.h"
#define MAX_BYTE_MSG_BODY 65535

class Message
{

private:

public:
unsigned Source=0;
unsigned int ID2=0;
unsigned int Sequence=0;
unsigned TargetLen=0;
unsigned MessageLen=0;
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
