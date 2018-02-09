#include "MessageHandler.h"
#include <string.h>
#include <stdio.h>

MessageHandler::MessageHandler(RFModem *current_modem,bool Monitoring_mode)
{
    modem=current_modem;
    Monitoring=Monitoring_mode;
    packethandler.Init(modem,Monitoring);
}

MessageHandler::~MessageHandler()
{

}

int MessageHandler::SetMessageSequence(unsigned char MsgSequence)
{
    MessageSequence=MsgSequence;
    return 0;
}

int MessageHandler::WaitForNextMessage()
{

    if(Monitoring)
    {
        if(packethandler.WaitForNextPacket()==1)
        {
                     
                     int res=  message.SetMessageFromPacket(&packethandler.rcvpacket);
                     if(res==0)  message.PrintState();
                     fprintf(stderr,"\n");
                     
                
           
        }
    }
    else
    {
        if(packethandler.WaitForNextPacket()==1)
        {
                     //packethandler.packet.PrintState();
                     int res=  message.SetMessageFromPacket(&packethandler.rcvpacket);
                     if(res==0)  message.PrintState();
                     fprintf(stderr,"\n");
                     
           
        }
    }
    return 0;
}

int MessageHandler::TxMessage()
{
   
    return 0;
}

int MessageHandler::TxMessageWaitAck(int MaxRetry=10)
{
   
    return 0;
    
}
