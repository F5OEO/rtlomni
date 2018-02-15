#include "PDMBolus.h"
#include <math.h>
#include <string.h>
#include <stdio.h>


PDMBolus::PDMBolus()
{
   
}

void PDMBolus::SetFromSubMessage(SubMessage *submessage_in)
{
    submessage.Len=submessage_in->Len;
    memcpy(submessage.Body,submessage_in->Body,submessage.Len);
}


int PDMBolus::InterpertSubmessage()
{

    
    return 0;
        
}

int PDMBolus::PrintState()
{
                     
    return 0;
}

int PDMBolus::Create(float Units,unsigned int Nonce,bool DividedFieldA)
{

    submessage.Type=0x1a;
    submessage.Len=14;

    submessage.Body[0]=Nonce>>24;
    submessage.Body[1]=Nonce>>16;
    submessage.Body[2]=Nonce>>8;
    submessage.Body[3]=Nonce&0xFF;

    submessage.Body[4]=0x2; // BOLUS TYPE
    
    submessage.Body[7]=1; // Duration 30 minutes
    unsigned int FieldA=0;
    if(DividedFieldA)
        FieldA=round(Units*8.0/0.05);
    else
        FieldA=round(Units*16.0/0.05);    
     
    submessage.Body[8]=FieldA>>8;
    submessage.Body[9]=FieldA&0xFF;
    unsigned int UnitRate=round(Units/0.05);
    submessage.Body[10]=UnitRate>>8;
    submessage.Body[11]=UnitRate&0xFF;
    submessage.Body[12]=UnitRate>>8;
    submessage.Body[13]=UnitRate&0xFF;

    unsigned int CheckSum=0;    
    for(int i=7;i<14;i++)
    {
        CheckSum+=submessage.Body[i];
    }
    CheckSum=CheckSum&0xFFFF;
    submessage.Body[5]=CheckSum>>8;
    submessage.Body[6]=CheckSum&0xFF;

    return 0;
}

int PDMBolus::CreateExtra(float Units,bool Pairing) 
{   
    submessage.Type=0x17;
    submessage.Len=13;
    //Correct : 00 02 08 00 01 86 a0 00 00 00 00 00 00
    //False : 1f108958ae1f108958181f1a0e7926c76d02010a0101a000340034 170d 00 02 08 00 00 18 6a f8
    unsigned int UnitExtra=round(Units/(0.1*0.05));
    submessage.Body[0]=0;
    submessage.Body[1]=UnitExtra>>8;
    submessage.Body[2]=UnitExtra&0xFF;
    submessage.Body[3]=0;
    if(Pairing)
    {
        
        submessage.Body[4]=0x01;//Unkown -> 200000 in decimal
        submessage.Body[5]=0x86;
        submessage.Body[6]=0xA0;
        submessage.Body[7]=0x0;
        submessage.Body[8]=0x0;
        submessage.Body[9]=0x0;
        submessage.Body[10]=0x0;
        submessage.Body[11]=0x0;
        submessage.Body[12]=0x0;
    }
    else
    {
        submessage.Body[4]=0x03;//Unkown -> 200000 in decimal
        submessage.Body[5]=0x0D;
        submessage.Body[6]=0x40;
        submessage.Body[7]=0x0;
        submessage.Body[8]=0x0;
        submessage.Body[9]=0x0;
        submessage.Body[10]=0x0;
        submessage.Body[11]=0x0;
        submessage.Body[12]=0x0;   
    }
 
    return 0;
}






