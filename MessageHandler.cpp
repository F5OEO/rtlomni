#include "MessageHandler.h"
#include "SubMessage.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

MessageHandler::MessageHandler(RFModem *current_modem,bool Monitoring_mode)
{
    modem=current_modem;
    Monitoring=Monitoring_mode;
    packethandler.Init(modem,Monitoring);
}

MessageHandler::~MessageHandler()
{

}

int MessageHandler::SetMessageSequence(int MsgSequence)
{
    if(MsgSequence<0) MsgSequence+=16;
    MessageSequence=MsgSequence%16;
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
                     if(res==0)
                     {
                         MessageSequence=message.Sequence;
                         
                         ParseSubMessage();
                     }
                     return res;
                     
           
        }
    }
    return -1;
}


int MessageHandler::ParseSubMessage()
{
     
                          
                          
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
                                    //Lotid=podpairing.Lot;
                                    //Tid=podpairing.Tid;
                                    
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
             if(messcomplete==0)
            {
                  message.PrintState();
                  MessageSequence=message.Sequence;              
                  ParseSubMessage();
            }
            
           while(messcomplete!=0)
           {
                messcomplete=WaitForNextMessage();
           }
           printf("Receive complete POD message\n");
            
            //Be sure there is no POD message who wait for ACK not heard
            int Count=0;
           while(Count<4)
           {
                if(packethandler.WaitForNextPacket()==-2) 
                    Count++;
                else
                    printf("Still some POD messages\n");
           }
            
           packethandler.Sequence=(packethandler.Sequence+1)%32; //READY FOR Next message to send  
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
    fprintf(stderr,"Msg Seq=%d ->",MessageSequence);
    SetMessageSequence(MessageSequence+1);
    fprintf(stderr,"Msg Seq cmd=%d\n",MessageSequence);

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
    
    return TxMessage(); 

}

int MessageHandler::VerifyPairing(unsigned long TargetID2)
{
    ID1=0xFFFFFFFF;
    ID2=0xFFFFFFFF;
    packethandler.SetTxAckID(ID1,TargetID2);
    SetMessageSequence(0);

    PDMVerifyPairing cmdpdmverifypairing;
    cmdpdmverifypairing.Create(TargetID2,Lotid,Tid);
    message.Reset();
    cmdpdmverifypairing.submessage.AttachToMessage(&message);
    cmdpdmverifypairing.submessage.AddToMessage();
    
    return TxMessage(); 

}

int MessageHandler::FinishPairing(unsigned long TargetID2)
{
    ID1=ID2=TargetID2;

    packethandler.SetTxAckID(ID1,ID2);
    SetMessageSequence(2);

    PDMCancelTime cmdpdmcanceltime;
    
    nonce.SyncNonce(Lotid,Tid,0);
    unsigned long FirstNonce=nonce.GetNounce(0);
    
    cmdpdmcanceltime.Create(FirstNonce,0);
    message.Reset();
    cmdpdmcanceltime.submessage.AttachToMessage(&message);
    cmdpdmcanceltime.submessage.AddToMessage();
    
    return TxMessage(); 

}

int MessageHandler::FinishPairing2(unsigned long TargetID2)
{
    ID1=ID2=TargetID2;
    
    packethandler.SetTxAckID(ID1,ID2);
    SetMessageSequence(4);
    PDMCancelTime cmdpdmcanceltime;
    
    nonce.SyncNonce(Lotid,Tid,0);
    unsigned long FirstNonce=nonce.GetNounce(1);
    cmdpdmcanceltime.Create(FirstNonce,1);
    message.Reset();
    cmdpdmcanceltime.submessage.AttachToMessage(&message);
    cmdpdmcanceltime.submessage.AddToMessage();
    
    return TxMessage(); 

}

int MessageHandler::Purging()
{
    SetMessageSequence(6);
    PDMBolus cmdpdmbolus;
    
    nonce.SyncNonce(Lotid,Tid,0);
    unsigned long ComputeNonce=nonce.GetNounce(2); 
    cmdpdmbolus.Create(2.6,ComputeNonce,true);
    message.Reset();
    cmdpdmbolus.submessage.AttachToMessage(&message);
    cmdpdmbolus.submessage.AddToMessage();
    cmdpdmbolus.CreateExtra(2.6,true);
    cmdpdmbolus.submessage.AddToMessage();
    
     return TxMessage(); 
    
}

int MessageHandler::FinishPurging()
{
    SetMessageSequence(8);
    PDMCancelTime cmdpdmcanceltime;
    
    nonce.SyncNonce(Lotid,Tid,0);
    unsigned long FirstNonce=nonce.GetNounce(3);
    cmdpdmcanceltime.Create(FirstNonce,2);
    message.Reset();
    cmdpdmcanceltime.submessage.AttachToMessage(&message);
    cmdpdmcanceltime.submessage.AddToMessage();
    
    return TxMessage(); 
    
}

int MessageHandler::BeginInjection()
{
    SetMessageSequence(10);
    PDMBasal cmdpdmbasal;
    
    nonce.SyncNonce(Lotid,Tid,0);
    unsigned long ComputeNonce=nonce.GetNounce(4); 
    cmdpdmbasal.Create(0,ComputeNonce,true);
    message.Reset();
    cmdpdmbasal.submessage.AttachToMessage(&message);
    cmdpdmbasal.submessage.AddToMessage();
    cmdpdmbasal.CreateExtra(0,true);
    cmdpdmbasal.submessage.AddToMessage();
    
     return TxMessage(); 
}

int MessageHandler::FinishInjection()
{
    SetMessageSequence(12);
    PDMCancelTime cmdpdmcanceltime;
    
    nonce.SyncNonce(Lotid,Tid,0);
    unsigned long FirstNonce=nonce.GetNounce(5);
    cmdpdmcanceltime.Create(FirstNonce,3);
    message.Reset();
    cmdpdmcanceltime.submessage.AttachToMessage(&message);
    cmdpdmcanceltime.submessage.AddToMessage();
    
    return TxMessage(); 
}

int MessageHandler::FinishInjection2()
{
   
    SetMessageSequence(14);
    PDMBolus cmdpdmbolus;
    
    nonce.SyncNonce(Lotid,Tid,0);
    unsigned long ComputeNonce=nonce.GetNounce(6); 
    cmdpdmbolus.Create(0.5,ComputeNonce,true);
    message.Reset();
    cmdpdmbolus.submessage.AttachToMessage(&message);
    cmdpdmbolus.submessage.AddToMessage();
    cmdpdmbolus.CreateExtra(0.5,true);
    cmdpdmbolus.submessage.AddToMessage();
    
    return TxMessage(); 
    
}

int MessageHandler::Bolus(float units)
{
    packethandler.SetTxAckID(ID1,0);
    SetMessageSequence(MessageSequence+1);
    int RememberSeq=MessageSequence;
    PDMBolus cmdpdmbolus;
    cmdpdmbolus.Create(units,0x851072aa,false);
    message.Reset();
    cmdpdmbolus.submessage.AttachToMessage(&message);
    cmdpdmbolus.submessage.AddToMessage();
    cmdpdmbolus.CreateExtra(units,false);
    cmdpdmbolus.submessage.AddToMessage();
    int res= TxMessage();
    if(res==0) 
    {
        sleep(1);
        SetMessageSequence(RememberSeq); // Exceptionnaly same msg sequence
        nonce.SyncNonce(Lotid,Tid,PODSeed.Seed);
        unsigned long ComputeNonce=nonce.GetNounce(0);
        cmdpdmbolus.Create(units,ComputeNonce,false);
        message.Reset();
        cmdpdmbolus.submessage.AttachToMessage(&message);
        cmdpdmbolus.submessage.AddToMessage();
        cmdpdmbolus.CreateExtra(units,false);
        cmdpdmbolus.submessage.AddToMessage();
        return TxMessage();
    }
    return -1;
}



