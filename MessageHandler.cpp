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
                          message.PrintState();
                          fprintf(stderr,"\n");
                          
                          int IndexInMessage=0;
                          int res=0;  
                          do
                          { 
                             SubMessage submessage(&message);
                             MessageSequence=message.Sequence;
                             res=submessage.ParseSubMessage(message.Body+IndexInMessage,message.TargetLen-IndexInMessage);
                             if(res!=-1) IndexInMessage+=res;   
                             if(submessage.Len>0)
                            { 
                                 submessage.PrintState();   
                                 if(submessage.Type==0x1D) 
                                {
                                    PODStatus.SetFromSubMessage(&submessage);
                                    PODStatus.InterpertSubmessage();
                                    PODStatus.PrintState();
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
    //message.Reset();
    packethandler.txack.ID1=ID1;
        
    
    message.ID2=ID2;
    message.Sequence=MessageSequence;
    message.Source=PDM;
    message.PacketizeMessage(ID1,packethandler.Sequence);
    int res=0;
    for(int i=0;i<message.packet_list_len;i++)
    {
        res=packethandler.TxPacketWaitAck(&message.packet_list[i],5,(i==message.packet_list_len-1));
    }
    if (res==1)
    {
           int messcomplete=message.SetMessageFromPacket(&packethandler.rcvpacket);
           while(messcomplete!=0)
           {
                messcomplete=WaitForNextMessage();
           } 
    }
    else
    {
          printf("Tx Message Failed\n");  
    }
    return 0;
}

int MessageHandler::TxMessageWaitAck(int MaxRetry=10)
{
   
    return 0;
    
}
