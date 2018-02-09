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
            int Len=modem->Receive(Frame,400);
            if(Len>0)
            {
                rcvpacket.SetPacketFromFrame(Frame,Len);
                if(rcvpacket.IsValid)
                {
                    //rcvpacket.PrintState();
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
                    
                    if(Sequence==0xFF) Sequence=(rcvpacket.Sequence+31)%32; // Init , never has a paquet sequence, set to the first seen
                    if(rcvpacket.Sequence==(Sequence+1)%32)
                    {
                        Sequence=rcvpacket.Sequence;
                        if(Transmitting)
                        {
                             if((rcvpacket.Type==PDM)||(rcvpacket.Type==CON)) continue; //Do not receive or own commands
                        }
                        return(1);
                    }
                    else // Bad sequence Number : surely a previous packet not acknowledge
                    {
                       // if(!Transmitting) TxAck(rcvpacket.Sequence);
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

int PacketHandler::TxPacket()
{
    unsigned char FrameTx[MAX_BYTE_PER_PACKET];
    int len=txpacket.GetFrame(FrameTx);
    modem->Transmit(FrameTx,len);
    return 0;
}

int PacketHandler::TxPacketWaitAck(int MaxRetry=10)
{
    Transmitting=true;
    int TxStatus=0;
    for(int i=0;(TxStatus!=1)&&(i<MaxRetry);i++)
    {
        TxPacket();
        TxStatus=WaitForNextPacket();
    }
    
    return TxStatus;
    
}
