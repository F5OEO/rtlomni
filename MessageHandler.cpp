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

void MessageHandler::SetLotTid(unsigned long TheLot,unsigned long TheTid)
{
    Lotid=TheLot;
    Tid=TheTid;
    PODSeed.SetLotTid(Lotid,Tid);
}

int MessageHandler::WaitForNextMessage()
{

    if(Monitoring)
    {
        if(packethandler.WaitForNextPacket()==1)
        {
                     
                     int res=  message.SetMessageFromPacket(&packethandler.rcvpacket);
                     message.PrintState();
                     fprintf(stderr,"\n");
                      if(res==0) ParseSubMessage();
                     
                     
                
           
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
                     if(res==0) ParseSubMessage();
                     
           
        }
    }
    return 0;
}


int MessageHandler::ParseSubMessage()
{
     
                          
                          
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
                                #define ANSI_COLOR_GREEN   "\x1b[32m"
                                #define ANSI_COLOR_RESET   "\x1b[0m"
                                fprintf(stderr,ANSI_COLOR_GREEN);   
                                 if(submessage.Type==0x1D) 
                                {
                                    PODStatus.SetFromSubMessage(&submessage);
                                    PODStatus.InterpertSubmessage();
                                    PODStatus.PrintState();
                                }
                                if(submessage.Type==0x06)
                                {
                                    PODSeed.SetFromSubMessage(&submessage,MessageSequence);
                                    PODSeed.InterpertSubmessage();
                                    PODSeed.PrintState();
                                }
                                if(submessage.Type==0x01)
                                {
                                   podpairing.SetFromSubMessage(&submessage);
                                    podpairing.InterpertSubmessage();
                                    podpairing.PrintState();
                                    Lotid=podpairing.Lot;
                                    Tid=podpairing.Tid;
                                    
                                    // Add update ID2 / LotID /TID
                                }
                                fprintf(stderr,ANSI_COLOR_RESET);   
                            }
                          }
                          while(res!=-1);   
    return 0;   
                 
}

int MessageHandler::TxMessage()
{
    
    //packethandler.txack.ID1=ID1;
        
    
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
             if(messcomplete==0)  message.PrintState();
             fprintf(stderr,"\n");
             if(messcomplete==0) ParseSubMessage();
            
           while(messcomplete!=0)
           {
                messcomplete=WaitForNextMessage();
           } 
    }
    else
    {
          printf("Tx Message Failed\n");
          return -1;    
    }
    return 0;
}

int MessageHandler::TxMessageWaitAck(int MaxRetry=10)
{
   
    return 0;
    
}

int MessageHandler::GetPodState(int TypeState)
{
    packethandler.SetTxAckID(ID1,0);
    PDMGetState cmdgetstate;
    cmdgetstate.Create(TypeState);
    message.Reset();
    cmdgetstate.submessage.AttachToMessage(&message);
    cmdgetstate.submessage.AddToMessage();
    return TxMessage(); 
}

int MessageHandler::Pairing(unsigned long TargetID2)
{
    ID1=0xFFFFFFFF;
    ID2=0xFFFFFFFF;
    packethandler.SetTxAckID(ID1,TargetID2);

    SetMessageSequence(0);
    packethandler.Sequence=0;

    PDMPairing cmdpdmpairing;
    cmdpdmpairing.Create(TargetID2);
    message.Reset();
    cmdpdmpairing.submessage.AttachToMessage(&message);
    cmdpdmpairing.submessage.AddToMessage();
    //TxACk should be filled with ID2 instead of zero afterward
    return TxMessage(); 

}

int MessageHandler::VerifyPairing(unsigned long TargetID2)
{
    ID1=0xFFFFFFFF;
    ID2=0xFFFFFFFF;
    packethandler.SetTxAckID(ID1,TargetID2);

    PDMVerifyPairing cmdpdmverifypairing;
    cmdpdmverifypairing.Create(TargetID2,Lotid,Tid);
    message.Reset();
    cmdpdmverifypairing.submessage.AttachToMessage(&message);
    cmdpdmverifypairing.submessage.AddToMessage();
    //TxACk should be filled with ID2 instead of zero afterward
    return TxMessage(); 

}

