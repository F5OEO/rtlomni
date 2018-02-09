
#ifndef _SUBMESSAGE
#define _SUBMESSAGE

#include "Message.h"

#define MAX_BYTE_SUBMSG_BODY 255

class SubMessage
{
private:
Message *message;
unsigned char Body[MAX_BYTE_SUBMSG_BODY];

public:
unsigned char Type=0;
unsigned char Len=0;
SubMessage(Message *message);
~SubMessage();
void PrintState();
int SetSubMessage();
int AddToMessage();
};
#endif
