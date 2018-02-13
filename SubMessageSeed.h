
#ifndef _SUBMESSAGESEED
#define _SUBMESSAGESEED

#include "SubMessage.h"

class SubMessageSeed
{
    private:
   
    SubMessage submessage;
    int Type=0;
    unsigned long Tid=0;
     unsigned long Lot=0;
    public:
    int Seed;
    int SeqMessagePOD;

    SubMessageSeed();
    void SetLotTid(unsigned long TheLot,unsigned long TheTid);
    void SetFromSubMessage(SubMessage *submessage_in,int SeqMessagePOD);
    int InterpertSubmessage();
    int PrintState();
};
#endif
