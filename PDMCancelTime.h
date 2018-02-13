
#ifndef _PDMCANCELTIME
#define _PDMCANCELTIME

#include "Message.h"
#include "SubMessage.h"
#define MAX_BYTE_SUBMSG_BODY 255

class PDMCancelTime
{
    private:
   
    public:
    SubMessage submessage;
    unsigned long ID2;
    
    PDMCancelTime();
    void SetFromSubMessage(SubMessage *submessage_in);
    int InterpertSubmessage();
    int PrintState();
    int Create(unsigned long Nonce, int Type);
};
#endif
