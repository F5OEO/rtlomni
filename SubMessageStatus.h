
#ifndef _SUBMESSAGESTATUS
#define _SUBMESSAGESTATUS

#define MAX_BYTE_SUBMSG_BODY 255
#include "SubMessage.h"

class SubMessageStatus
{
    private:
   
    SubMessage submessage;
    public:
    int InsulinState;
    int PoDState;
    int TotalUnit;
    int LastCommandMsgSeq;
    int InsulinNotDelivered;
    int Alarm;
    int MinutesActive;
    int ReservoirLevel;

    SubMessageStatus();
    void SetFromSubMessage(SubMessage *submessage_in);
    int InterpertSubmessage();
    int PrintState();
};
#endif
