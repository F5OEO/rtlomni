#include "PacketHandler.h"
#include <string.h>
#include <stdio.h>

PacketHandler::PacketHandler(RFModem *current_modem)
{
    modem=current_modem;
}

PacketHandler::~PacketHandler()
{
}

int PacketHandler::WaitForNextPacket()
{

    while(1)
    {
        int Len=modem->Receive(Frame,400);
        if(Len>0)
        {
            packet.SetPacketFromFrame(Frame,Len);
            if(packet.IsValid)
            {
                if(Sequence==0xFF) Sequence=(packet.Sequence+31)%32; // Init , never has a paquet sequence, set to the first seen
                if(packet.Sequence==(Sequence+1)%32)
                {
                    Sequence=packet.Sequence;
                    return(1);
                }
                else // Bad sequence Number : surely a previous packet not acknowledge
                {
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
