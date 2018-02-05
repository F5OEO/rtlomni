
#include "RFModem.h"


void *ReadSDR(void * arg)
{
    RFModem *Modem=(RFModem *)arg;
    while(1)
        {
            int Result=Modem->ProcessRF();
            if(Result==2) 
            {
                sem_post(&sem_receive);
            }
            if(Result==0) break;       
        }
        return NULL;
}

void *WriteSDR(void * arg)
{
    return NULL;
}

RFModem::RFModem()
{
    InitRF();
};

RFModem::~RFModem()
{
};

int RFModem::SetIQFile(char *IQFileName,int Direction)
{
    if(Direction==0)//Receive
    {
         iqfilein=fopen(IQFileName, "r");
         if(iqfilein==NULL) {printf("Can't open %s\n",IQFileName);return(-1);};
         pthread_create (&thReadSDR,NULL, &ReadSDR,this);
    }
    if(Direction==1)//Transmit
    {
         iqfileout=fopen (IQFileName, "wb");
         if(iqfilein==NULL) {printf("Can't open %s\n",IQFileName);return(-1);};
         pthread_create (&thWriteSDR,NULL, &WriteSDR,this);
    }
    return 0;
}





//RF Process is 
// Get U8 IQ sample at 1.3Msymbols (32*Omnipod symbol rate)
// In order to remove DC spike, tuning of receiver is 325KHz above   
// 1 : 1.3M U8 IQ -> 1.3M Complex float
// 2 : NCO +325Khz
// 3 : Decimation by 4
// 4 : FM Demodulation 
// 5 : Decision with number of transition inside a window of 8 sample
// Todo : Between 2 and 3, make a low pass filtering to avoid replicant signals
// After 3: Make a matched filter (correlator) with conjugate signal of SYNC signal 

void RFModem::InitRF()
{    

     float IQSR=1300000.0;  // 32*Baudrate

    // Create NCO
    float FreqUp= 325000.0+5000;
    MyNCO = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(MyNCO, 0.0f);
    nco_crcf_set_frequency(MyNCO, 2.0*M_PI*FreqUp/IQSR); // Tuning frequency is SR/4 away : here 256/4=64KHZ : 433923+64=433987

    // Create decimator by 4
    
    int          type       = LIQUID_RESAMP_DECIM;
    unsigned int num_stages = 2;        // decimate by 2^2=4
    float        fc         =  0.2f;    // signal cut-off frequency
    float        f0         =  0.0f;    // (ignored)
    float        As         = 60.0f;    // stop-band attenuation
    MyDecim = msresamp2_crcf_create(type, num_stages, fc, f0, As);


    float BaudRate=40625.0;
    // FM Demodulation 
    float FSKDeviationHz=26296.0;//26370.0; //Inspectrum show +/-20KHZ ?  
   
    fdem=freqdem_create(FSKDeviationHz*2*4/IQSR);
    
    //FSK modulator 
    fmod = fskmod_create(1,8,FSKDeviationHz/(8*BaudRate));  // 1 bit by symbol / 8 IQ samples by symbol (upsample*8), Deviation : DeviationHz/(Upsample*Baudrate)

    ReceiveSampleBySymbol      =   IQSR/40625;     // filter samples/symbol -> Baudrate
    buf_rx=(liquid_float_complex*)malloc(ReceiveSampleBySymbol*sizeof(liquid_float_complex));
    iq_buffer=(uint8_t *)malloc(ReceiveSampleBySymbol*2*sizeof(uint8_t)); // 1Byte I, 1Byte Q
    
      
}


//***************************************************************************************************
//*********************************** RECEIVE PART **************************************************
//***************************************************************************************************


int RFModem::Receive(unsigned char *Frame)
{
    sem_wait (&sem_receive);
    memcpy(Frame,BufferData,IndexData);
    return IndexData;
}


int RFModem::AddData(unsigned char DataValue)
{
   static int CountHeader=0;
    //printf("\nIndexdata%d->%x:%x\n",IndexData,DataValue^0xFF,DataValue);
    if((IndexData==0)&&DataValue==0x54) {CountHeader++;return 1;} //Skip SYNC BYTE : 0x54 is No t inverted
    if((IndexData==0)&&DataValue==0xC3) {/*printf("_");*/return 1;} //Skip 2SYNC BYTE : C3 is not inverted
    if(IndexData==0) {/*printf("CountHeader=%d\n",CountHeader);*/CountHeader=0;}
    if(IndexData<MAXPACKETLENGTH)
        BufferData[IndexData++]=DataValue^0xFF;
    else
    {   
        IndexData=MAXPACKETLENGTH-1; 
        printf("Packet too long !!!\n");
        return 0;
    }
    return 1;
    
}

int RFModem::ManchesterAdd(int BitValue)	
{
    static int prev=-1;
    
    char bitDecoded=-1;
    static int IndexInByte=0;
    static int DecodedByte=0;
  


    if(BitValue==-1) {prev=-1;IndexInByte=0;DecodedByte=0;return -1;} //Reset Manchester  
        //printf("%d",BitValue);
    int FinalByteValue=-1;
    if(prev==-1) { prev=BitValue; return -1;};
    if((prev==0) && (BitValue==1)) {bitDecoded=0;prev=-1;}
    if((prev==1) && (BitValue==0)) {bitDecoded=1;prev=-1;}
    if(prev==BitValue) {/*printf("\n!%d\n",IndexInByte);*/prev=-1;return -2;}
    if(bitDecoded>=0)
    {
        //printf(" M%d ",bitDecoded);
        
        DecodedByte=(DecodedByte<<1)|bitDecoded;
        //DecodedByte=(DecodedByte)bitDecoded<<(7-IndexInByte);
        if(IndexInByte<7)
        {
            IndexInByte++;
            FinalByteValue=-1; //In decoding state 
        }
        else
        {
            IndexInByte=0;
            FinalByteValue=(DecodedByte)&0xFF;
            //printf(".");    
           //printf("->%x\n",FinalByteValue);
            DecodedByte=0;
        }
    }
    else
    {
        IndexInByte=0;
        DecodedByte=0;
        FinalByteValue=-1;
    }
    
    return FinalByteValue;//

}

//***************************************************************************************************
//*********************************** FSK LAYER *****************************************************
//***************************************************************************************************

  

#define FSK_SYNC_RUNNING 0
#define FSK_SYNC_ON 1
#define FSK_SYNC_OFF 2



int RFModem::GetFSKSync(unsigned char Sym)
{
    static int Index=0;
    static unsigned int Buffer=0;

    int FSKCurrentStatus=FSK_SYNC_RUNNING;
   
    Buffer=((Buffer<<1)&0xFFFE)|Sym;
    
    if(Buffer==0x6665) {/*printf("#");*/Buffer=0;FSKCurrentStatus=FSK_SYNC_ON;}
    if((Buffer&0xF)==0xF) FSKCurrentStatus=FSK_SYNC_OFF;        
    
    return(FSKCurrentStatus);
}
   
int RFModem::ProcessRF()
{
                
              unsigned int i=0;
              unsigned int j;
            static unsigned int SampleTime=0; 
             int bytes_read=0;
              static  int Lock=-1;
           float resultautocor=0;
             bytes_read = fread(iq_buffer, 1, ReceiveSampleBySymbol*2, iqfilein);
          
            if(StatusModem!=Status_Receive) return 0; 
            
           if (bytes_read ==ReceiveSampleBySymbol*2)
           {
                                
                // convert u8 to f32 and NCO
                for (j=0, i=0; j<bytes_read; j+=2, i++) 
                {
                    liquid_float_complex r= complex<float>((((uint8_t*)iq_buffer)[j] -127.5)/128.0,(((uint8_t*)iq_buffer)[j+1] -127.5)/128.0);
                    liquid_float_complex rdown;
                    nco_crcf_step(MyNCO);
                    nco_crcf_mix_up(MyNCO, r, &rdown);
                    buf_rx[i]=rdown;
                }
                liquid_float_complex AfterDecim[ReceiveSampleBySymbol/4];
                float FmAmplitude[ReceiveSampleBySymbol/4];    

                for(i=0;i<ReceiveSampleBySymbol/4;i++) //Decimation by 4 and FM demodulate
                {
                    msresamp2_crcf_execute(MyDecim, &buf_rx[i*4], &AfterDecim[i]);  
                    freqdem_demodulate(fdem, AfterDecim[i], &FmAmplitude[i]);
                                       
                    //float re=crealf(buf_rx2[i]);//printf("%f+i)",re);
                    //float im=cimagf(buf_rx2[i]);//printf("%f\n",im);
                   


                }
                static int FMState=0;
                static int SampleFromLastTransition=0;    
                int NbSymbol=0;
                unsigned char Sym[ReceiveSampleBySymbol/4];
                for(i=0;i<ReceiveSampleBySymbol/4;i++) 
                {
                    SampleFromLastTransition++;
                    if((FMState==0)&&(FmAmplitude[i]>=0.4))
                         {Sym[NbSymbol++]=1;FMState=1;SampleFromLastTransition=0;}
                    else
                        if((FMState==1)&&(FmAmplitude[i]<-0.4)) {Sym[NbSymbol++]=0;FMState=0;SampleFromLastTransition=0;}   
                    
                    if(SampleFromLastTransition>(ReceiveSampleBySymbol/4+2)) {Sym[NbSymbol++]=FMState;SampleFromLastTransition=0;} 
                }
                
                if((NbSymbol>2)||(NbSymbol==0)) return 1;/* else printf("%d",NbSymbol);*/// More than 2 transition is surely noise    
                
                
                for(int i=0;i<NbSymbol;i++)
                {
                    static int FSKSyncStatus=0;
                    
                     //sym_out=(sym_out==0)?1:0;

                    if(FSKSyncStatus==1)
                    {
                        
                        int Manchester=ManchesterAdd(Sym[i]);
                        //
                        if(Manchester>=0)
                        {
                                unsigned char ManchesterByte=Manchester; 
                                //fwrite(&ManchesterByte,1,1,ManchesterFile);
                                if(AddData(Manchester)==0) //Packet is too long !!!
                                {
                                    FSKSyncStatus=0; 
                                    ManchesterAdd(-1);
                                   return(2);
                                };
                                
                        }
                        else
                        {
                            
                            if(Manchester==-2)
                            {
                                //printf("\n Unlock \n");
                                //ParsePacket(SampleTime*1e3/IQSR); //ms
                                
                                
                                FSKSyncStatus=0; // Error in Manchester 
                                //IndexData=0;
                                ManchesterAdd(-1);
                                
                                return(2);
                                
                            }
                            
                                              
                        }                        
              
                    }
                    else
                        ManchesterAdd(-1);
                                
                    //if(FSKSyncStatus!=1)
                    {
                        int InTimeSync=GetFSKSync(Sym[i]);
                        if(FSKSyncStatus==0)
                        {
                            switch(InTimeSync)
                            {
                                case FSK_SYNC_ON: FSKSyncStatus=1; IndexData=0;break;
                                case FSK_SYNC_OFF:FSKSyncStatus=0;break;
                            }
                        }
                    }
                }    
              
                

                
                return 1;
                    
          } 
          else
              return 0;               

}

//***************************************************************************************************
//*********************************** TX FUNCTIONS **************************************************
//***************************************************************************************************

int RFModem::Transmit(unsigned char *Frame,unsigned int Length)
{
    if(Length>MAXPACKETLENGTH) return -1;
    
    WriteSync();
    for(int i=0;i<Length;i++) WriteByteManchester(Frame[i],1);
    WriteEnd(); 
    return 0;
}


void RFModem::WriteFSKIQ(unsigned char bit)
{
   //printf("TxSymbolsSize %d\n",TxSymbolsSize);
     
   TxSymbols[TxSymbolsSize++]=bit; 
   if(TxSymbolsSize>(MAX_BYTE_PER_PACKET+200+10)*8*2) 
    {
        //printf("\n Tx Overflow :%d\n",TxSymbolsSize);
        TxSymbolsSize=0; 
    }
       
}



void RFModem::WriteByteManchester(unsigned char Byte,char flip)
{
    unsigned char ByteFlip;
    if(flip==1) ByteFlip=Byte^0xFF; else ByteFlip=Byte; 
    //printf("%x",ByteFlip);
    for(int i=7;i>=0;i--)
    {
        if(((ByteFlip>>i)&0x1)==0) 
        {
            WriteFSKIQ(0);
            WriteFSKIQ(1);    
        }
        else
        {
            WriteFSKIQ(1);
            WriteFSKIQ(0);
        }
    }
}

void RFModem::WriteSync()
{
    //TxSymbolsSize=0; // Should alread by reset by last sending
    if(TxSymbolsSize!=0) printf("Tx overflow ????\n");
    for(int i=0;i<200;i++)
    {
        WriteByteManchester(0x54,0);
        //WriteByteManchester(0xFF,0);
    }
    WriteByteManchester(0xC3,0);    
}


void RFModem::WriteEnd()
{
       for(int i=0;i<10;i++)
        {
           WriteFSKIQ(1);
        }
        
}


