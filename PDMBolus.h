
#ifndef _PDMBOLUS
#define _PDMBOLUS

#include "Message.h"
#include "SubMessage.h"
#define MAX_BYTE_SUBMSG_BODY 255

class PDMBolus
{
    private:
   
    public:
    SubMessage submessage;
    unsigned long ID2;
    
    PDMBolus();
    void SetFromSubMessage(SubMessage *submessage_in);
    int InterpertSubmessage();
    int PrintState();
    int Create(float Units,unsigned int Nonce,bool DividedFieldA);
    int CreateExtra(float Units,bool Pairing);
};
#endif
