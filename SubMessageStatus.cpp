#include "SubMessageStatus.h"
#include <string.h>
#include <stdio.h>


SubMessageStatus::SubMessageStatus()
{
   
}

void SubMessageStatus::SetFromSubMessage(unsigned char *SubMessageBody,unsigned char SubmessageLen)
{
    Len=SubmessageLen;
    memcpy(Body,SubMessageBody,Len);
}


int SubMessageStatus::InterpertSubmessage()
{
    if(Len>=9)
            {
                        printf(":");
                        //byte0: bits ABCDEEEE. Bits A, B, C, D indicate values of internal table 7. 4-bit value EEEE is an important internal state value. 
                        switch(Body[0]>>4)
                        {
                            case 0:printf("Insulin stopped");break;
                            case 1:printf("Basal running");break;
                            case 2:printf("Temp Basal running");break;
                            case 4:printf("Purgin");break;
                            case 5:printf("Bolusing");break;
                            case 6:printf("Bolusing+Tempbasal");break;
                            default:printf("Unknown %x",Body[0]>>4);break;
                        } 
                        printf(" ");  
                        //printf("Table7:");printbit(Body[0],0,3);printf(" ");
                        printf("PODState:");
                        int PodStateTable7=Body[0]&0xF;
                        switch(PodStateTable7)
                        {
                            case 3:printf("Pairing success");break;
                            case 4:printf("Purging");break;
                            case 5:printf("Ready for injection");break;
                            case 6:printf("Injection done");break;
                            case 8:printf("Normal running");break;
                            case 9:printf("Running with low insulin");break;
                            case 15:printf("Inactive");break;
                            default:printf("Unknown %d",PodStateTable7);  
                        
                        };
                        printf(" ");
                        //dword: 4 zero bits. 13 bits with Table1[2]. 4 bits (maybe message sequence number). 11 bits (sum of various Table entries divided by 10 and rounded up).
                        //printf("4zero:");printbit(Body[1],4,7);printf(" ");

                        //printf("Table1[2]:");printbit(Body[1],0,3);printbit(Body[2],0,7);printbit(Body[3],7,7);printf(" ");//4bits+8bits+1bit=13bits : Table1[2]
                        int Table12=((Body[1]&0xF)<<9)|(Body[2]<<1)|(Body[3]>>7);
                        //printf("Table1[2]:%x ",Table12);
                        printf("Insulin(total):%0.2fu ",Table12*0.05);
                        //printf("seqnumb:");printbit(Body[3],3,6);printf(" ");//4bits    
                        int ResponseMessageFromPod=(Body[3]>>3)&0xF;
                        printf("PODMsg#%d ",ResponseMessageFromPod);
                        //printbit(Body[3],0,2);printbit(Body[4],0,7);printf(" "); //3+8=11bits   
                        int SumTable=((Body[3]&0x3)<<8)|Body[4];

                        //printf("sum table(*10):%d ",SumTable*10);
                        printf("Insulin not delivered:%0.2fu",SumTable*0.05);
                        printf(" ");
               
                        //dword:1 bit (indicates event 0x14 was logged) 8 bits (internal value) 13 bits Pod time active (Tab1[1]) 10 bits Reservoir level (Tab1[0])
                        int Event14=Body[5]>>7;
                        if(Event14)
                        {
                             printf("Event14:%x",Event14);
                           
                        }

                        printf("Alert:");
                        
                        int InternalValue=((Body[5]&0x7F)<<1)|(Body[6]>>7);
                        switch(InternalValue)
                        {
                              case 0x8:printf("Perim soon");break;
                              case 0x80:printf("Replace POD");break;  
                              case 0x82:printf("Replace PODEx");break;    
                              case 0x20:printf("End of cancel insulin in 15 minutes");break; //beep
                              case 0x40:printf("End of cancel insulin now");break;  //beep beeeeep beep beeeeeep
                              case 0:printf("Normal");break;
                              default:printf("Unknown x%x",InternalValue);break;
                        }
                        printf(" ");    
                        
                        int MinutesActives=((Body[6]&0x7F)<<6)|((Body[7]>>2)&0x3F);
                        int Days=MinutesActives/60/24;
                        int Hours=(MinutesActives-Days*60*24)/60;
                        int Minutes=(MinutesActives-Days*60*24-Hours*60);
                        printf("POD Active for %d days %d hours %d minutes",Days,Hours,Minutes);printf(" ");//7+6  
                        int Reservoir=(((Body[7]&0x03)<<8)+(Body[8]));
                        if((Reservoir&0xFF)!=0xFF)
                            printf("Reservoir Level %0.01fU",(((Body[7]&0x03)<<8)|(Body[8]))*50.0/1024.0);  
                       printf("\n");
                        

            }
    else 
        printf("Len <9:%d\n",Len);
    return 0;
        
}


