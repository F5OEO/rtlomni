#include "PDMBasal.h"
#include <math.h>
#include <string.h>
#include <stdio.h>


PDMBasal::PDMBasal()
{
   
}

void PDMBasal::SetFromSubMessage(SubMessage *submessage_in)
{
    submessage.Len=submessage_in->Len;
    memcpy(submessage.Body,submessage_in->Body,submessage.Len);
}


int PDMBasal::InterpertSubmessage()
{

    
    return 0;
        
}

int PDMBasal::PrintState()
{
                     
    return 0;
}

int PDMBasal::Create(float Units,unsigned int Nonce,bool DividedFieldA)
{

    submessage.Type=0x1a;
    submessage.Len=22;

    submessage.Body[0]=Nonce>>24;
    submessage.Body[1]=Nonce>>16;
    submessage.Body[2]=Nonce>>8;
    submessage.Body[3]=Nonce&0xFF;


    //00 01 dc 22 2c c0 00 03 10 04 b8 03 f8 04 f8 04 18 04
        submessage.Body[4]=0x00;
        submessage.Body[5]=0x01;
        submessage.Body[6]=0xdc;
        submessage.Body[7]=0x22;
        submessage.Body[8]=0x2c;
        submessage.Body[9]=0xc0;
        submessage.Body[10]=0x00;
        submessage.Body[11]=0x03;
        submessage.Body[12]=0x10;
        submessage.Body[13]=0x04;
        submessage.Body[14]=0xB8;
        submessage.Body[15]=0x03;
        submessage.Body[16]=0xF8;
        submessage.Body[17]=0x04;
        submessage.Body[18]=0xF8;
        submessage.Body[19]=0x04;
        submessage.Body[20]=0x18;
        submessage.Body[21]=0x04;

    return 0;
}

int PDMBasal::CreateExtra(float Units,bool Pairing) 
{   
    submessage.Type=0x13;
    submessage.Len=26;
   
    //40 02 02 6d 01 e8 48 00 00 2d 02 62 5a 00 01 c7 03 10 bc db 05 fa 02 62 5a 00
         submessage.Body[0]=0x40;
        submessage.Body[1]=0x02;
        submessage.Body[2]=0x02;
        submessage.Body[3]=0x6d;
       
        submessage.Body[4]=0x01;
        submessage.Body[5]=0xE8;
        submessage.Body[6]=0x48;
        submessage.Body[7]=0x00;
        submessage.Body[8]=0x00;
        submessage.Body[9]=0x2d;
        submessage.Body[10]=0x02;
        submessage.Body[11]=0x62;
        submessage.Body[12]=0x5a;
        submessage.Body[13]=0x00;
        submessage.Body[14]=0x01;
        submessage.Body[15]=0xC7;
        submessage.Body[16]=0x03;
        submessage.Body[17]=0x10;
        submessage.Body[18]=0xBC;
        submessage.Body[19]=0xDB;
        submessage.Body[20]=0x05;
        submessage.Body[21]=0xFA;
        submessage.Body[22]=0x02;
        submessage.Body[23]=0x62;
        submessage.Body[24]=0x5A;
        submessage.Body[25]=0x00;
    return 0;
}






