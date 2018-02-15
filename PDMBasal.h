
#ifndef _PDMBASAL
#define _PDMBASAL

#include "Message.h"
#include "SubMessage.h"
#define MAX_BYTE_SUBMSG_BODY 255

class PDMBasal
{
    private:
   
    public:
    SubMessage submessage;
    unsigned long ID2;
    
    PDMBasal();
    void SetFromSubMessage(SubMessage *submessage_in);
    int InterpertSubmessage();
    int PrintState();
    int Create(float Units,unsigned int Nonce,bool DividedFieldA);
    int CreateExtra(float Units,bool Pairing);
};
#endif
