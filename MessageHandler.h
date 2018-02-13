#ifndef _MESSAGE_HANDLER
#define _MESSAGE_HANDLER

#include "Message.h"
#include "PacketHandler.h"
#include "SubMessageStatus.h"
#include "SubMessageSeed.h"

class MessageHandler
{

private:
RFModem *modem;
unsigned char MessageSequence=0xFF;

bool Monitoring=true;
bool Transmitting=false;


public:
PacketHandler packethandler;
unsigned long ID1;
unsigned long ID2;
Message message;
SubMessageStatus PODStatus;
SubMessageSeed PODSeed;
unsigned long Lotid;
unsigned long Tid;

MessageHandler(RFModem *current_modem,bool Monitoring_mode=true);
~MessageHandler();

 void SetLotTid(unsigned long TheLot,unsigned long TheTid);
int SetMessageSequence(unsigned char MsgSequence);
int WaitForNextMessage();
int ParseSubMessage();

int TxMessage();
int TxMessageWaitAck(int MaxRetry);
};
#endif
