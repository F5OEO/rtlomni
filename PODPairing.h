
#ifndef _PODPAIRING
#define _PODPAIRING

#include "SubMessage.h"

class PodPairing
{
    private:
   
    SubMessage submessage;
    int Type=0;
    
    public:
    
    unsigned long Tid=0;
    unsigned long Lot=0;
    unsigned long ID2=0;
    unsigned long PM=0;
    unsigned long PI=0;
    int State=0;
    int Rssi=0;

    PodPairing();
    void SetFromSubMessage(SubMessage *submessage_in);
    int InterpertSubmessage();
    int PrintState();
};
#endif
