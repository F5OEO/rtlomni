#include "PODPairing.h"
#include <string.h>
#include <stdio.h>

PodPairing::PodPairing()
{
   
}



void PodPairing::SetFromSubMessage(SubMessage *submessage_in)
{
    
    submessage.Len=submessage_in->Len;
    memcpy(submessage.Body,submessage_in->Body,submessage.Len);
}


int PodPairing::InterpertSubmessage()
{

    if(submessage.Len<0x15) return -1;
            if(submessage.Len==0x1b)//27
            {
                PM=(submessage.Body[7]<<16)|(submessage.Body[8]<<8)|(submessage.Body[9]);
                PI=(submessage.Body[10]<<16)|(submessage.Body[11]<<8)|(submessage.Body[12]);
                State=submessage.Body[14];
                Lot=(submessage.Body[15]<<24)|(submessage.Body[16]<<16)|(submessage.Body[17]<<8)|submessage.Body[18];
                Tid=(submessage.Body[19]<<24)|(submessage.Body[20]<<16)|(submessage.Body[21]<<8)|submessage.Body[22];
                ID2= (submessage.Body[23]<<24)|(submessage.Body[24]<<16)|(submessage.Body[25]<<8)|submessage.Body[26];
                
                
            }

            if(submessage.Len==0x15)//21    
            {
                PM=(submessage.Body[0]<<16)|(submessage.Body[1]<<8)|(submessage.Body[2]);
                PI=(submessage.Body[3]<<16)|(submessage.Body[4]<<8)|(submessage.Body[5]);
                State=submessage.Body[7];
                Lot=(submessage.Body[8]<<24)|(submessage.Body[9]<<16)|(submessage.Body[10]<<8)|submessage.Body[11];
                Tid=(submessage.Body[12]<<24)|(submessage.Body[13]<<16)|(submessage.Body[14]<<8)|submessage.Body[15];
                Rssi=submessage.Body[16];
                ID2=(submessage.Body[17]<<24)|(submessage.Body[18]<<16)|(submessage.Body[19]<<8)|submessage.Body[20];
               
            }
         
    return 0;
        
}

int PodPairing::PrintState()
{
    if(submessage.Len==0x1B)
        fprintf(stderr,"PodPairing : LotId %ld Tid %ld State = %d ID2=%lx PM=%d.%d.%d PI=%d.%d.%d Rssi=%x \n",Lot,Tid,State,ID2,(int)((PM>>16)&0xFF),(int)((PM>>8)&0xFF),(int)((PM&0xFF)),(int)((PI>>16)&0xFF),(int)((PI>>8)&0xFF),(int)(PI&0xFF),Rssi); 
     if(submessage.Len==0x15)
         fprintf(stderr,"PodPairing : LotId %ld Tid %ld State = %d ID2=%lx PM=%d.%d.%d PI=%d.%d.%d  \n",Lot,Tid,State,ID2,(int)((PM>>16)&0xFF),(int)((PM>>8)&0xFF),(int)((PM&0xFF)),(int)((PI>>16)&0xFF),(int)((PI>>8)&0xFF),(int)(PI&0xFF));  
    return 0;
}


