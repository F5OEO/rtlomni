#include "MessageHandler.h"
#include "SubMessage.h"
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
                     if(res==0)
                    {
                          //message.PrintState();
                          //fprintf(stderr,"\n");
                          
                          int IndexInMessage=0;
                          int res=0;  
                          do
                          { 
                             SubMessage submessage(&message);
                             
                             res=submessage.ParseSubMessage(message.Body+IndexInMessage,message.TargetLen-IndexInMessage);
                             if(res!=-1) IndexInMessage+=res;   
                             if(submessage.Len>0)
                            { 
                                 submessage.PrintState();   
                                 if(submessage.Type==0x1D) 
                                {
                                    PODStatus.SetFromSubMessage(submessage.Body,submessage.Len);
                                    PODStatus.InterpertSubmessage();
                                }
                            }
                          }
                          while(res!=-1);      
                    }
                     
                     
                
           
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
