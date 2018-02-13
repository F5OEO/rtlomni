#include "PDMVerifyPairing.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

PDMVerifyPairing::PDMVerifyPairing()
{
   
}

void PDMVerifyPairing::SetFromSubMessage(SubMessage *submessage_in)
{
    submessage.Len=submessage_in->Len;
    memcpy(submessage.Body,submessage_in->Body,submessage.Len);
}


int PDMVerifyPairing::InterpertSubmessage()
{

    
    return 0;
        
}

int PDMVerifyPairing::PrintState()
{
    printf("Request PDM Paring %lx",ID2);                    
    return 0;
}

int PDMVerifyPairing::Create(unsigned long ID2Request,unsigned long TheLot,unsigned long TheTid)
{
    ID2=ID2Request;
    Lot=TheLot;
    Tid=TheTid;

     submessage.Type=0x3;
    submessage.Body[0]=ID2>>24;
    submessage.Body[1]=ID2>>24;
    submessage.Body[2]=ID2>>24;
    submessage.Body[3]=ID2>>24;
    submessage.Body[4]=0x0; // Ignored
    submessage.Body[5]=0x4; // Unknown

    time_t t = time(NULL);
    
    struct tm *tm = localtime(&t);
  
    submessage.Body[6]=tm->tm_mon+1; 
    submessage.Body[7]=tm->tm_mday;
    submessage.Body[8]=tm->tm_year-100; // year from 2000
    submessage.Body[9]=tm->tm_hour;
    submessage.Body[10]=tm->tm_min;
    
    // Lot
    submessage.Body[11]=Lot>>24;
    submessage.Body[12]=Lot>>16;
    submessage.Body[13]=Lot>>8;
    submessage.Body[14]=Lot&0xFF;

    //Tid
    submessage.Body[15]=Tid>>24;
    submessage.Body[16]=Tid>>16;
    submessage.Body[17]=Tid>>8;
    submessage.Body[18]=Tid&0xFF;
    submessage.Len=19;
    return 0;
}



