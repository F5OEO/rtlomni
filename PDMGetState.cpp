#include "PDMGetState.h"
#include <string.h>
#include <stdio.h>


PDMGetState::PDMGetState()
{
   
}

void PDMGetState::SetFromSubMessage(SubMessage *submessage_in)
{
    submessage.Len=submessage_in->Len;
    memcpy(submessage.Body,submessage_in->Body,submessage.Len);
}


int PDMGetState::InterpertSubmessage()
{

    TypeState=submessage.Body[0];  
    return 0;
        
}

int PDMGetState::PrintState()
{
    printf("Request POD State type %x",TypeState);                    
    return 0;
}

int PDMGetState::Create(int Type)
{
    submessage.Type=0xE;
    TypeState=Type;
    submessage.Body[0]=TypeState;
    submessage.Len=1;
    return 0;
}



