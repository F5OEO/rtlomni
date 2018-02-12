
#ifndef _PDMGETSTATE
#define _PDMGETSTATE

#include "Message.h"
#include "SubMessage.h"
#define MAX_BYTE_SUBMSG_BODY 255

class PDMGetState
{
    private:
   
    public:
    SubMessage submessage;
    int TypeState;
    
    PDMGetState();
    void SetFromSubMessage(SubMessage *submessage_in);
    int InterpertSubmessage();
    int PrintState();
    int Create(int Type);
};
#endif
