
#ifndef _SUBMESSAGESTATUS
#define _SUBMESSAGESTATUS

#define MAX_BYTE_SUBMSG_BODY 255

class SubMessageStatus
{
    private:
    unsigned char Body[MAX_BYTE_SUBMSG_BODY];
    unsigned char Len=0;
    public:
    SubMessageStatus();
    void SetFromSubMessage(unsigned char *SubMessageBody,unsigned char SubmessageLen);
    int InterpertSubmessage();
};
#endif
