#include "SubMessage.h"
#include <string.h>
#include <stdio.h>

SubMessage::SubMessage()
{
    
}

SubMessage::SubMessage(Message *messagein)
{
    AttachToMessage(messagein);
}

SubMessage::~SubMessage()
{
}

void SubMessage::AttachToMessage(Message *messagein)
{
    message=messagein;    
}

int SubMessage::AddToMessage()
{
    message->AddToBody(&Type,1);
    message->AddToBody(&Len,1);
    message->AddToBody(Body,Len);
    return 0;
}

void SubMessage::PrintState()
{
    fprintf(stderr,"SubMsg Layer:");    
    switch(message->Source)
    {
       case PDM:fprintf(stderr,"PDM ");break;
       case POD:fprintf(stderr,"POD ");break;
      
       default:fprintf(stderr,"UNKOWN ");break;         
    } 
    fprintf(stderr,"Type:%02x ",Type);
    fprintf(stderr,"Len:%d ",Len);
    for(int i=0;i<Len;i++) fprintf(stderr,"%02x",Body[i]);
   fprintf(stderr,"\n");

}

int SubMessage::ParseSubMessage(unsigned char *MsgBody,int LenBody)
{
    if(LenBody<2) return -1;
    Type=MsgBody[0];
    if (Type==0x1D)
    {
        Len=LenBody-1;
        memcpy(Body,MsgBody+1,Len);
        return Len+1;
    }
    else 
    {
        Len=MsgBody[1];
        memcpy(Body,MsgBody+2,Len);
        return Len+2;
    }
    return -1;
}

int SubMessage::SetSubMessage()
{
    return 0;
}



