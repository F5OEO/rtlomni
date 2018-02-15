#include "PDMCancelTime.h"
#include <string.h>
#include <stdio.h>


PDMCancelTime::PDMCancelTime()
{
   
}

void PDMCancelTime::SetFromSubMessage(SubMessage *submessage_in)
{
    submessage.Len=submessage_in->Len;
    memcpy(submessage.Body,submessage_in->Body,submessage.Len);
}


int PDMCancelTime::InterpertSubmessage()
{

    ID2=(submessage.Body[0]<<24)|(submessage.Body[1]<<16)|(submessage.Body[2]<<8)|(submessage.Body[0]);  
    return 0;
        
}

int PDMCancelTime::PrintState()
{
    printf("Request PDM CancelTime %lx",ID2);                    
    return 0;
}

int PDMCancelTime::Create(unsigned long Nonce, int Type)
{

    submessage.Type=0x19;
    submessage.Len =10;
    submessage.Body[0]=Nonce>>24;
    submessage.Body[1]=Nonce>>16;
    submessage.Body[2]=Nonce>>8;
    submessage.Body[3]=Nonce&0xFF;
//
    if(Type==0)
    {
        submessage.Body[4]=0x4c;    
        submessage.Body[5]=0x00;
        submessage.Body[6]=0x00;
        submessage.Body[7]=0x64;
        submessage.Body[8]=0x01;
        submessage.Body[9]=0x02;
    }
    if(Type==1)
    {
        submessage.Body[4]=0x78;    
        submessage.Body[5]=0x37;
        submessage.Body[6]=0x00;
        submessage.Body[7]=0x05;
        submessage.Body[8]=0x08;
        submessage.Body[9]=0x02;
    }
    if(Type==2)
    {
        submessage.Body[4]=0x38;    
        submessage.Body[5]=0x00;
        submessage.Body[6]=0x10;
        submessage.Body[7]=0xA3;
        submessage.Body[8]=0x03;
        submessage.Body[9]=0x02;
    }
    if(Type==3)
    {

        // //79a410df0502280012830602020f00000202
         //79 a4 10 df 05 02 28 00 12 83 06 02 02 0f 00 00 02 02
        submessage.Body[4]=0x79;    
        submessage.Body[5]=0xa4;
        submessage.Body[6]=0x10;
        submessage.Body[7]=0xdf;
        submessage.Body[8]=0x05;
        submessage.Body[9]=0x02;
        submessage.Body[10]=0x28;
        submessage.Body[11]=0x00;
        submessage.Body[12]=0x12;
        submessage.Body[13]=0x83;
        submessage.Body[14]=0x06;
        submessage.Body[15]=0x02;
        submessage.Body[16]=0x02;
        submessage.Body[17]=0x0F;
        submessage.Body[18]=0x00;
        submessage.Body[19]=0x00;
        submessage.Body[20]=0x02;    
        submessage.Body[21]=0x02;
        submessage.Len=22;
    }
   
    return 0;
}



