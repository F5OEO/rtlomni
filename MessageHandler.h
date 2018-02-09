#ifndef _MESSAGE_HANDLER
#define _MESSAGE_HANDLER

#include "Message.h"
#include "PacketHandler.h"



class MessageHandler
{

private:
RFModem *modem;
unsigned char MessageSequence=0xFF;

bool Monitoring=true;
bool Transmitting=false;
PacketHandler packethandler;
public:
Message message;


MessageHandler(RFModem *current_modem,bool Monitoring_mode=true);
~MessageHandler();

int SetMessageSequence(unsigned char MsgSequence);
int WaitForNextMessage();

int TxMessage();
int TxMessageWaitAck(int MaxRetry);
};
#endif
