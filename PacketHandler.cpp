#include "PacketHandler.h"
#include <string.h>
#include <stdio.h>


PacketHandler::PacketHandler()
{
}


PacketHandler::PacketHandler(RFModem *current_modem,bool Monitoring_mode)
{
    Init(current_modem,Monitoring_mode);
    
}

PacketHandler::~PacketHandler()
{
}

void PacketHandler::Init(RFModem *current_modem,bool Monitoring_mode)
{
    modem=current_modem;
    Monitoring=Monitoring_mode;
}

int PacketHandler::WaitForNextPacket()
{
    if(Monitoring)
    {
        while(1)
        {
            int Len=modem->Receive(Frame,600);
            if(Len>0)
            {
                rcvpacket.SetPacketFromFrame(Frame,Len);
                if(rcvpacket.IsValid)
                {
                    rcvpacket.PrintState();
                    if(Sequence==0xFF) Sequence=(rcvpacket.Sequence+31)%32; // Init , never has a paquet sequence, set to the first seen
                    if(rcvpacket.Sequence==(Sequence+1)%32)
                    {
                        Sequence=rcvpacket.Sequence;
                        return(1);
                    }
                    else // Bad sequence Number : surely a previous packet not acknowledge
                    {
                        
                        if(rcvpacket.Sequence>Sequence)
                        {
                         fprintf(stderr,"Bad packet Seq %d but waited %d\n",rcvpacket.Sequence,(Sequence+1)%32);   
                         Sequence=rcvpacket.Sequence; // FixMe roll up with modulo 32 
                         return(1);   
                        }
                        return(-1);
                    }
                }
            }
            else // Timeout
            {
                return(-2);
            }
        }
    }
    else
    {
        while(1)
        {
            int Len=modem->Receive(Frame,400);
            if(Len>0)
            {
                rcvpacket.SetPacketFromFrame(Frame,Len);
                
                if(rcvpacket.IsValid)
                {
                    //if(Transmitting) printf("Tx\n"); else printf("Rx\n");
                    rcvpacket.PrintState();
                    if(Sequence==0xFF) Sequence=(rcvpacket.Sequence+31)%32; // Init , never has a paquet sequence, set to the first seen
                    if(rcvpacket.Sequence==(Sequence+1)%32)
                    {
                        Sequence=rcvpacket.Sequence;
                        if(Transmitting)
                        {
                             if((rcvpacket.Type==PDM)||(rcvpacket.Type==CON)) continue; //Do not receive or own commands
                             
                        }
                        else
                        {
                            if((rcvpacket.Type==ACK)) continue; //Do not receive or own commands
                            TxAck((rcvpacket.Sequence+1)%32);//answer to POD/CON 
                        }
                        return(1);
                    }
                    else // Bad sequence Number : surely a previous packet not acknowledge
                    {
                        Sequence=rcvpacket.Sequence;
                        if(Transmitting)
                        {
                             if((rcvpacket.Type==PDM)||(rcvpacket.Type==CON)) continue; //Do not receive or own commands
                        }
                        else
                        {
                            if((rcvpacket.Type==ACK)) continue; //Do not receive or own commands
                            TxAck((rcvpacket.Sequence+1)%32);//answer to POD/CON 
                        }
                        printf("Bad seq number rcv %d / wanted %d\n",rcvpacket.Sequence,(Sequence+1)%32);
        
                        
                        //if(!Transmitting) TxAck((rcvpacket.Sequence+1)%32);
                        return(-1);
                    }
                }
            }
            else // Timeout
            {
                return(-2);
            }
        }
    }
    
}


int PacketHandler::SetTxAckID(unsigned long AckID1,unsigned long AckID2)
{
    txack.ID1=AckID1;
    txack.Type=ACK;
    txack.PacketLen=4;
    txack.Body[0]=AckID2>>24;
    txack.Body[1]=AckID2>>16;
    txack.Body[2]=AckID2>>8;
    txack.Body[3]=AckID2;
    return 0;
}

int PacketHandler::TxAck(int AckSequence)
{
    txack.Sequence=AckSequence;
    Sequence=AckSequence;    
    fprintf(stderr,"Tx ACK %d\n",AckSequence);
    TxPacket(&txack,true);
    
    return 0;
}
int PacketHandler::TxPacket(Packet *packet_to_tx,bool ShortSync)
{
    unsigned char FrameTx[MAX_BYTE_PER_PACKET];
    int len=packet_to_tx->GetFrame(FrameTx);
    modem->Transmit(FrameTx,len,ShortSync);
    return 0;
}

int PacketHandler::TxPacketWaitAck(Packet *packet_to_tx,int MaxRetry,bool LastPacket)
{
    Transmitting=true;
    int TxStatus=0;
    for(int i=0;(TxStatus!=1)&&(i<MaxRetry);i++)
    {
        TxPacket(packet_to_tx,false);
        TxStatus=WaitForNextPacket();
        
    }
    if(LastPacket)
    {
        
        if(TxStatus==1)
        {
             TxAck((rcvpacket.Sequence+1)%32);//answer to POD/CON 
             //Sequence=(rcvpacket.Sequence+1)%32; //ACK
            Transmitting=false;
        }        
    }
    return TxStatus;
    
}
