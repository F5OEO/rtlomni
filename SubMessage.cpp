#include "SubMessage.h"
#include <string.h>
#include <stdio.h>

SubMessage::SubMessage(Message *message)
{
}

SubMessage::~SubMessage()
{
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
    

}

int SubMessage::SetSubMessage()
{
    return 0;
}



