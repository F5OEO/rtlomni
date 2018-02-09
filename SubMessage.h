
#ifndef _SUBMESSAGE
#define _SUBMESSAGE

#include "Message.h"

#define MAX_BYTE_SUBMSG_BODY 255

class SubMessage
{
protected:
Message *message;


public:
unsigned char Body[MAX_BYTE_SUBMSG_BODY];
unsigned char Type=0;
unsigned char Len=0;
SubMessage(Message *message);
~SubMessage();
void PrintState();
int ParseSubMessage(unsigned char *MsgBody,int LenBody);

int SetSubMessage();
int AddToMessage();
};
#endif
