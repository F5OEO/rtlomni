
#ifndef _PDMPAIRING
#define _PDMPAIRING

#include "Message.h"
#include "SubMessage.h"
#define MAX_BYTE_SUBMSG_BODY 255

class PDMPairing
{
    private:
   
    public:
    SubMessage submessage;
    unsigned long ID2;
    
    PDMPairing();
    void SetFromSubMessage(SubMessage *submessage_in);
    int InterpertSubmessage();
    int PrintState();
    int Create(unsigned long ID2);
};
#endif
