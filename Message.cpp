#include "Message.h"
#include <string.h>
#include <stdio.h>

Message::Message()
{
}

Message::~Message()
{
}


//Packet Layer(1518695192.877):POD ID1:ffffffff Seq:4 PktLen:31 crc8:fd(OK)ff ff ff ff 04 1d 01 1b 13 88 10 08 34 0a 50 02 07 00 02 07 00 02 03 00 00 a5 5f 00 09 ed 72
//Tx ACK 5
//Packet Layer(1518695193.38):ACK ID1:ffffffff Seq:5 PktLen:4 crc8:89(OK)1f108958
//Packet Layer(1518695193.140):CON ID1:ffffffff Seq:6 PktLen:6 crc8:27(OK)1f10895882b0
//Tx ACK 7
//Msg Layer:POD ID2:ffffffff Seq:1 Len:29/29 crc16:82b0/82b0 (OK)011b13881008340a5002070002070002030000a55f0009 1f10895882b0


int Message::SetFirst(Packet *packet)
{
    Reset();
    Source=packet->Type;
    ID2=(packet->Body[0]<<24)|(packet->Body[1]<<16)|(packet->Body[2]<<8)|(packet->Body[3]);
    Sequence=(packet->Body[4]&0x3F)>>2;
    TargetLen=(packet->Body[5]|((packet->Body[4]&3)<<8));
    memcpy(Body,packet->Body+6,packet->PacketLen-6);//Message Header(6) - CRC16 (2)
    MessageLen+=packet->PacketLen-6;

    memcpy(CompleteRawMessage,packet->Body,packet->PacketLen);
    RawLen+=packet->PacketLen;
    return 0;
}

int Message::SetCon(Packet *packet)
{
    memcpy(Body+MessageLen,packet->Body,packet->PacketLen);
    MessageLen+=packet->PacketLen;

    memcpy(CompleteRawMessage+RawLen,packet->Body,packet->PacketLen);
    RawLen+=packet->PacketLen;
    return 0;
}


int Message::SetMessageFromPacket(Packet *packet)
{
    switch(packet->Type)
    {
        case POD:SetFirst(packet);break;
        case PDM:SetFirst(packet);break;
        case ACK:return -1;break;
        case CON:SetCon(packet);break; 
        default:break;   
    }
    
    if((TargetLen+2)==MessageLen)
    {
        crc16=(CompleteRawMessage[RawLen-2]<<8)|CompleteRawMessage[RawLen-1];
        if(computecrc16(CompleteRawMessage,RawLen-2)==crc16) IsValid=true;
        MessageLen=MessageLen-2; //We remove CRC16
        
        if(IsValid==true) return 0; else fprintf(stderr,"CRC16 ERROR\n");
    }
    return -1;
}

//*************************************DATA DEFINITION AND CRC***********************************************
unsigned int crc_table16[] = {0x0000,0x8005,0x800f,0x000a,0x801b,0x001e,0x0014,0x8011,0x8033,
               0x0036,0x003c,0x8039,0x0028,0x802d,0x8027,0x0022,0x8063,0x0066,
               0x006c,0x8069,0x0078,0x807d,0x8077,0x0072,0x0050,0x8055,0x805f,
               0x005a,0x804b,0x004e,0x0044,0x8041,0x80c3,0x00c6,0x00cc,0x80c9,
               0x00d8,0x80dd,0x80d7,0x00d2,0x00f0,0x80f5,0x80ff,0x00fa,0x80eb,
               0x00ee,0x00e4,0x80e1,0x00a0,0x80a5,0x80af,0x00aa,0x80bb,0x00be,
               0x00b4,0x80b1,0x8093,0x0096,0x009c,0x8099,0x0088,0x808d,0x8087,
               0x0082,0x8183,0x0186,0x018c,0x8189,0x0198,0x819d,0x8197,0x0192,
               0x01b0,0x81b5,0x81bf,0x01ba,0x81ab,0x01ae,0x01a4,0x81a1,0x01e0,
               0x81e5,0x81ef,0x01ea,0x81fb,0x01fe,0x01f4,0x81f1,0x81d3,0x01d6,
               0x01dc,0x81d9,0x01c8,0x81cd,0x81c7,0x01c2,0x0140,0x8145,0x814f,
               0x014a,0x815b,0x015e,0x0154,0x8151,0x8173,0x0176,0x017c,0x8179,
               0x0168,0x816d,0x8167,0x0162,0x8123,0x0126,0x012c,0x8129,0x0138,
               0x813d,0x8137,0x0132,0x0110,0x8115,0x811f,0x011a,0x810b,0x010e,
               0x0104,0x8101,0x8303,0x0306,0x030c,0x8309,0x0318,0x831d,0x8317,
               0x0312,0x0330,0x8335,0x833f,0x033a,0x832b,0x032e,0x0324,0x8321,
               0x0360,0x8365,0x836f,0x036a,0x837b,0x037e,0x0374,0x8371,0x8353,
               0x0356,0x035c,0x8359,0x0348,0x834d,0x8347,0x0342,0x03c0,0x83c5,
               0x83cf,0x03ca,0x83db,0x03de,0x03d4,0x83d1,0x83f3,0x03f6,0x03fc,
               0x83f9,0x03e8,0x83ed,0x83e7,0x03e2,0x83a3,0x03a6,0x03ac,0x83a9,
               0x03b8,0x83bd,0x83b7,0x03b2,0x0390,0x8395,0x839f,0x039a,0x838b,
               0x038e,0x0384,0x8381,0x0280,0x8285,0x828f,0x028a,0x829b,0x029e,
               0x0294,0x8291,0x82b3,0x02b6,0x02bc,0x82b9,0x02a8,0x82ad,0x82a7,
               0x02a2,0x82e3,0x02e6,0x02ec,0x82e9,0x02f8,0x82fd,0x82f7,0x02f2,
               0x02d0,0x82d5,0x82df,0x02da,0x82cb,0x02ce,0x02c4,0x82c1,0x8243,
               0x0246,0x024c,0x8249,0x0258,0x825d,0x8257,0x0252,0x0270,0x8275,
               0x827f,0x027a,0x826b,0x026e,0x0264,0x8261,0x0220,0x8225,0x822f,
               0x022a,0x823b,0x023e,0x0234,0x8231,0x8213,0x0216,0x021c,0x8219,
               0x0208,0x820d,0x8207,0x0202};

unsigned int Message::computecrc16(unsigned char *data,int len)
{

    unsigned int acc = 0x00;
    

    for(int i=0;i<len;i++)
    {
    
        acc = (acc >> 8) ^ crc_table16[(acc ^ data[i]) & 0xff];
    }
    

    return acc;
}

void Message::PrintState()
{
    if((Source!=PDM)&&(Source!=POD)) return;
    fprintf(stderr,"Msg Layer:");    
    switch(Source)
    {
       case PDM:fprintf(stderr,"PDM ");break;
       case POD:fprintf(stderr,"POD ");break;
      
       default:fprintf(stderr,"UNKOWN ");break;         
    } 
    fprintf(stderr,"ID2:%08x ",ID2);
    fprintf(stderr,"Seq:%d ",Sequence);
    fprintf(stderr,"Len:%d/%d ",MessageLen,TargetLen);
    fprintf(stderr,"crc16:%04x/%04x ",crc16,computecrc16(CompleteRawMessage,RawLen-2));
    if(IsValid) fprintf(stderr,"(OK)"); else fprintf(stderr,"(KO)");
     for(int i=0;i<TargetLen;i++) fprintf(stderr,"%02x",Body[i]);
   fprintf(stderr,"\n");

}

int Message::Reset()
{
    MessageLen=0;TargetLen=0;ID2=0;crc16=0;Source=0;IsValid=false;RawLen=0;
    packet_list_len=0;
    return 0;
}


int Message::PacketizeMessage(unsigned int ID1,unsigned int PacketSequence)
{
    
    packet_list_len=0;

    
    CompleteRawMessage[0]=ID2>>24;
    CompleteRawMessage[1]=ID2>>16;
    CompleteRawMessage[2]=ID2>>8;
    CompleteRawMessage[3]=ID2&0xFF;
    
    CompleteRawMessage[4]=(Sequence<<2)|((MessageLen>>8)&0x3);
    CompleteRawMessage[5]=MessageLen&0xFF;

    memcpy(CompleteRawMessage+6,Body,MessageLen);

    unsigned int Messagecrc=computecrc16(CompleteRawMessage,MessageLen+6);
    CompleteRawMessage[MessageLen+6]=Messagecrc>>8;
    CompleteRawMessage[MessageLen+6+1]=Messagecrc&0xFF;
    
    int MessageLengthRemaining=MessageLen+6+2;
    int ByteSent=0;
    do
    {
        packet_list[packet_list_len].ID1=ID1;
        packet_list[packet_list_len].Sequence=PacketSequence;
        if(packet_list_len==0)
            packet_list[packet_list_len].Type=Source;
        else
            packet_list[packet_list_len].Type=CON;
        
        if(MessageLengthRemaining>(MAX_BYTE_BODY))
      
          packet_list[packet_list_len].PacketLen=MAX_BYTE_BODY;
        else
          packet_list[packet_list_len].PacketLen=MessageLengthRemaining;    
        memcpy(packet_list[packet_list_len].Body,CompleteRawMessage+ByteSent,packet_list[packet_list_len].PacketLen);
        ByteSent+= packet_list[packet_list_len].PacketLen;
        MessageLengthRemaining-=packet_list[packet_list_len].PacketLen;
        packet_list_len++;

        PacketSequence=(PacketSequence+2)%32; //+2 because POD should always answer with packet +1
       
    }
    while (MessageLengthRemaining>0);
    return packet_list_len;
    
}

int Message::AddToBody(unsigned char* SubMessage,unsigned int SubMessageLen)
{
    memcpy(Body+MessageLen,SubMessage,SubMessageLen);
    MessageLen+=SubMessageLen;
    return 0;
}


