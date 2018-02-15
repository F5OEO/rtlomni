#ifndef _NONCE
#define _NONCE
#include <stdio.h>
#include <stdlib.h>
#define MAX_NOUNCES_PROCESS 100

class Nonce
{
    private:
   unsigned long *TabNounce=NULL;
    unsigned long a7[18];
    unsigned long GenerateEntryNonce();
    public:
    
    
    Nonce();
    ~Nonce();
    int SyncNonce(unsigned long lot, unsigned long tid,int Seed);
    unsigned long GetNounce(int IndexNounce);
    int PrintState();
    
};
#endif
