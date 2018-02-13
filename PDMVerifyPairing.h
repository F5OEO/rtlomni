
#ifndef _PDMVERIFYPAIRING
#define _PDMVERIFYPAIRING

#include "Message.h"
#include "SubMessage.h"
#define MAX_BYTE_SUBMSG_BODY 255

class PDMVerifyPairing
{
    private:
   
    public:
    SubMessage submessage;
    unsigned long ID2;
    unsigned long Tid;
    unsigned long Lot;
    
    PDMVerifyPairing();
    void SetFromSubMessage(SubMessage *submessage_in);
    int InterpertSubmessage();
    int PrintState();
    int Create(unsigned long ID2Request,unsigned long TheLot,unsigned long TheTid);
};
#endif
