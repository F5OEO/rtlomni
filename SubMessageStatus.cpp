#include "SubMessageStatus.h"
#include <string.h>
#include <stdio.h>


SubMessageStatus::SubMessageStatus()
{
   
}

void SubMessageStatus::SetFromSubMessage(SubMessage *submessage_in)
{
    submessage.Len=submessage_in->Len;
    memcpy(submessage.Body,submessage_in->Body,submessage.Len);
}


int SubMessageStatus::InterpertSubmessage()
{

    
    if(submessage.Len>=9)
            {
                        
                        InsulinState=submessage.Body[0]>>4;
                        PoDState=submessage.Body[0]&0xF;
                        TotalUnit=((submessage.Body[1]&0xF)<<9)|(submessage.Body[2]<<1)|(submessage.Body[3]>>7);
                        LastCommandMsgSeq=(submessage.Body[3]>>3)&0xF;
                        InsulinNotDelivered=((submessage.Body[3]&0x3)<<8)|submessage.Body[4];
                        Alarm=((submessage.Body[5]&0x7F)<<1)|(submessage.Body[6]>>7);
                        MinutesActive=((submessage.Body[6]&0x7F)<<6)|((submessage.Body[7]>>2)&0x3F);
                        ReservoirLevel=(((submessage.Body[7]&0x03)<<8)+(submessage.Body[8]));
            }
    else 
    {    
        printf("Len <9:%d\n",submessage.Len);
        return -1;
    }
    return 0;
        
}

int SubMessageStatus::PrintState()
{
                        switch(InsulinState)
                        {
                            case 0:printf("Insulin stopped");break;
                            case 1:printf("Basal running");break;
                            case 2:printf("Temp Basal running");break;
                            case 4:printf("Purgin");break;
                            case 5:printf("Bolusing");break;
                            case 6:printf("Bolusing+Tempbasal");break;
                            default:printf("Unknown %x",InsulinState);break;
                        } 
                        printf(" ");  
                       
                        printf("PODState:");
                       
                        switch(PoDState)
                        {
                            case 3:printf("Pairing success");break;
                            case 4:printf("Purging");break;
                            case 5:printf("Ready for injection");break;
                            case 6:printf("Injection done");break;
                            case 8:printf("Normal running");break;
                            case 9:printf("Running with low insulin");break;
                            case 15:printf("Inactive");break;
                            default:printf("Unknown %d",PoDState);  
                        
                        };
                        printf(" ");
                      
                       
                        printf("Insulin(total):%0.2fu ",TotalUnit*0.05);
                     
                       
                        printf("PODMsg#%d ",LastCommandMsgSeq);
                       
                        printf("Insulin not delivered:%0.2fu",InsulinNotDelivered*0.05);
                        printf(" ");
                                       
                        printf("Alarm:");
                        
                       
                        switch(Alarm)
                        {
                              case 0x8:printf("Perim soon");break;
                              case 0x80:printf("Replace POD");break;  
                              case 0x82:printf("Replace PODEx");break;    
                              case 0x20:printf("End of cancel insulin in 15 minutes");break; //beep
                              case 0x40:printf("End of cancel insulin now");break;  //beep beeeeep beep beeeeeep
                              case 0:printf("Normal");break;
                              default:printf("Unknown x%x",Alarm);break;
                        }
                        printf(" ");    
                        
                       
                        int Days=MinutesActive/60/24;
                        int Hours=(MinutesActive-Days*60*24)/60;
                        int Minutes=(MinutesActive-Days*60*24-Hours*60);
                        printf("POD Active for %d days %d hours %d minutes",Days,Hours,Minutes);printf(" ");//7+6  
                        
                        if((ReservoirLevel&0xFF)!=0xFF)
                            printf("Reservoir Level %0.01fU",(ReservoirLevel)*50.0/1024.0);
                        else
                            printf("Reservoir Level over 50U");  
                       printf("\n");
    return 0;
}


