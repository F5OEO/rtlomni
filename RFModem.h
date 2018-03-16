
#ifndef _RFMODEM
#define _RFMODEM

#include <iostream>
#include <math.h>
#include <complex>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

using std::complex;
#include <liquid/liquid.h>
#include "./librpitx/src/librpitx.h"

#define MAX_BYTE_PER_PACKET (31+6)
#define MAXPACKETLENGTH MAX_BYTE_PER_PACKET

static sem_t sem_receive;
static sem_t sem_tx;

class RFModem {

private:

    static void *ReadSDR(void * arg);
    static void *WriteSDR(void * arg);
	fskburst *fskmod=NULL;
    //Common 
    int StatusModem=Status_Idle;
   
   //Rx 
    uint8_t* iq_buffer; // 1Byte I, 1Byte Q
        
    liquid_float_complex *buf_rx;

    nco_crcf MyNCO;
    msresamp2_crcf MyDecim;
    freqdem fdem;
    FILE* iqfilein=NULL;
    unsigned int ReceiveSampleBySymbol=0;
    unsigned char BufferData[MAXPACKETLENGTH];
    int IndexData=0;
    bool FrameIsPresent=0;

    pthread_t thReadSDR; // Thread read in RTLSDR
    

    //Tx
    
    
    #define MAX_SYMBOLS (MAX_BYTE_PER_PACKET+200+50)*8*2
    unsigned char TxSymbols[MAX_SYMBOLS]; // 8bits*2(MANCHESTER) 
    int TxSymbolsSize=0;
    
    liquid_float_complex buf_tx[MAX_SYMBOLS*8]; //8 samples by symbol
    int buf_tx_len=0;
    
    #define RPITX_BURST_SIZE 1000
    liquid_float_complex dummy_tx[RPITX_BURST_SIZE];
    
    pthread_mutex_t muttx = PTHREAD_MUTEX_INITIALIZER;
     pthread_t thWriteSDR; // Thread read in RTLSDR
   

private:
void InitRF();
int AddData(unsigned char DataValue);
int ManchesterAdd(int BitValue);
int GetFSKSync(unsigned char Sym);


void WriteFSKIQ(unsigned char bit);
void WriteByteManchester(unsigned char Byte,char flip);
void WriteSync(bool ShortPacket);
void WriteEnd();

public:
    RFModem();
    ~RFModem();
    int ProcessRF(); // Only public for thread    
    int SetIQFile(char *IQFileName,int Direction);
    int Receive(unsigned char *Frame,int Timeoutms);
    int Transmit(unsigned char *Frame,unsigned int Length,bool ShortPacket);
    enum {Status_Idle,Status_Receive,Status_Transmit};
    int SetStatus(int Status);
    
};
#endif
