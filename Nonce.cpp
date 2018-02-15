#include "Nonce.h"
#include <stdio.h>

Nonce::Nonce()
{
    TabNounce=(unsigned long*)malloc(MAX_NOUNCES_PROCESS*sizeof(unsigned long));
}

Nonce::~Nonce()
{
    free(TabNounce);
    TabNounce=NULL;
}

int Nonce::SyncNonce(unsigned long lot, unsigned long tid,int Seed)
{
        unsigned long Nonce=0;
        unsigned char  byte_F9=0; 
      
        a7[0]=(lot & 0xFFFF) + 0x55543DC3 + (lot >> 16);
        a7[0]&=0xFFFFFFFF;
        a7[1]=(tid & 0xFFFF) + 0xAAAAE44E + (tid >> 16);
        a7[1]&=0xFFFFFFFF;
       
        
       
        a7[0]+=(Seed&0xFF);
            

        for(int i=2;i<18;i++)
        {
           a7[i]=GenerateEntryNonce();      
        } 
        
        if(Seed==0)
            byte_F9 = (a7[0] + a7[1]) & 0xF; 
        else
            byte_F9=(a7[0] + a7[1]) & 0xF;
        for(int i=0;i<=MAX_NOUNCES_PROCESS;i++)
        {

            Nonce=a7[2+byte_F9];
            TabNounce[i]=Nonce;
            a7[2 + byte_F9] = GenerateEntryNonce();
            byte_F9=Nonce&0xF;

        }
        fprintf(stderr,"Set Nonce table for Lot %ld Tid %ld\n",lot,tid);
    return 0;
}

unsigned long Nonce::GenerateEntryNonce()
{
        a7[0] = ((a7[0] >> 16) + (a7[0] & 0xFFFF) * 0x5D7F) & 0xFFFFFFFF;
        a7[1] = ((a7[1] >> 16) + (a7[1] & 0xFFFF) * 0x8CA0) & 0xFFFFFFFF;
        return ((a7[1] + (a7[0] << 16)) & 0xFFFFFFFF);
}


unsigned long Nonce::GetNounce(int IndexNounce)
{
    unsigned long ProcessNounce=TabNounce[IndexNounce];
    fprintf(stderr,"Nonce(%d)= %lx\n",IndexNounce,ProcessNounce);   
    return ProcessNounce;
}

int Nonce::PrintState()
{
    
    return 0;
}
