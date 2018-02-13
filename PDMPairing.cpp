#include "PDMPairing.h"
#include <string.h>
#include <stdio.h>


PDMPairing::PDMPairing()
{
   
}

void PDMPairing::SetFromSubMessage(SubMessage *submessage_in)
{
    submessage.Len=submessage_in->Len;
    memcpy(submessage.Body,submessage_in->Body,submessage.Len);
}


int PDMPairing::InterpertSubmessage()
{

    ID2=(submessage.Body[0]<<24)|(submessage.Body[1]<<16)|(submessage.Body[2]<<8)|(submessage.Body[0]);  
    return 0;
        
}

int PDMPairing::PrintState()
{
    printf("Request PDM Paring %lx",ID2);                    
    return 0;
}

int PDMPairing::Create(unsigned long ID2Request)
{
    submessage.Type=0x7;
    ID2=ID2Request;
    submessage.Body[0]=ID2>>24;
    submessage.Body[1]=ID2>>16;
    submessage.Body[2]=ID2>>8;
    submessage.Body[3]=ID2;
    submessage.Len=4;
    return 0;
}



