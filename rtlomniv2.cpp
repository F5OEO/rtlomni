/* Created by Evariste Courjaud F5OEO (2017-2018). Code is GPL v2
rtlomni is a software to sniff RF packets using a RTLSDR dongle in order to analysis Omnipod protocol.

Credits :

This work is mainly based on https://github.com/ps2/omnipod_rf

Hope this could help and contribute to https://github.com/openaps/openomni

SDR demodulation and signal processing is based on excellent https://github.com/jgaeddert/liquid-dsp/

Licence : 
	
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "RFModem.h"
//#include "Packet.h"
#include "PacketHandler.h"
#include "Message.h"
#include "MessageHandler.h"

#include <getopt.h>
#include <unistd.h>


#define PROGRAM_VERSION "0.0.2"

void print_usage()
{

	fprintf(stderr,\
"rtlomni (%s)\n\
Usage:rtlomniv2 -i File [-l LotID] [-t Tid][-c][-m Mode][-a Address][-n Msg sequence][-p PacketSeq][-h] \n\
-i            Input File (RF type(.cu8) or RFTrace from RfCat \n\
-l            LotID \n\
-t            Tide \n\
-c            Colorize messages \n\
-d            Write copy of input RF packet(debug.cu8) \n\
-m            Mode {tx,rx} (rx by default) \n\
-a            Physical POD address in Hexa \n\
-n            Message Sequence Number to start with \n\
-p            Packet Sequence Number to start with \n\
-h            help (print this help).\n\
Example : ./rtlomni -i omniup325.cu8 for reading a capture file from rtlsdr\n\
Example : ./rtlomni -i badcrc.txt for reading a capture file from rfcat\n\
\n", \
PROGRAM_VERSION);

} /* end function print_usage */

int main(int argc, char **argv)
{
    int a;
	int anyargs = 0;
    char inputname[255];
    char outputname[255]="FSK.ft";
    bool Monitoring=1;
    int MessageSeqStart=0;
    int PacketSeqStart=0;
    unsigned long PhysicalPodAddress;
    unsigned long mlot=0;
    unsigned mtid=0;    
     while (1)
	{
		a = getopt(argc, argv, "i:l:t:cdm:a:n:p:h");

		if (a == -1)
		{
			if (anyargs) break;
			else a = 'h'; //print usage and exit
		}
		anyargs = 1;

		switch (a)
		{
		case 'i': // InputFile
			strcpy(inputname,optarg);	
			break;
         case 'm': // Mode Tx ou Rx
            if(strcmp(optarg,"tx")==0) Monitoring=false;        
			break;     
        case 'l': // Lot
			mlot=atol(optarg);
			break;
        case 't': // Tid
			 mtid=atol(optarg);
			break; 
        /*
        case 'c': // Colorize message
			 colorize=1;
			break;    
         case 'd': // Debug mode : recording I/Q
			 debugfileiq=1;
			break;      
       */
        case 'a': // Physical Adress (need for Tx)
            {
            char *p;
            PhysicalPodAddress=strtoul(optarg, &p, 16);
            }      
			break;  
        case 'n': // Message sequence
            MessageSeqStart=atoi(optarg);        
			break;  
        case 'p': // Packet Sequence
            PacketSeqStart=atoi(optarg);        
			break;   
         
		case 'h': // help
			print_usage();
			exit(0);
			break;
		case -1:
			break;
		case '?':
			if (isprint(optopt))
			{
				fprintf(stderr, "rtlomni `-%c'.\n", optopt);
			}
			else
			{
				fprintf(stderr, "rtlomni: unknown option character `\\x%x'.\n", optopt);
			}
			print_usage();

			exit(1);
			break;
		default:
			print_usage();
			exit(1);
			break;
		}/* end switch a */
	}/* end while getopt() */

    RFModem Modem;
    Modem.SetIQFile(inputname,0);
    if(!Monitoring)
    {
        Modem.SetIQFile(outputname,1);
    }
    unsigned char Frame[MAX_BYTE_PER_PACKET];
    
    //Message message; 
    //PacketHandler packethandler(&Modem,Monitoring);
    
    MessageHandler messagehandler(&Modem,Monitoring);
    messagehandler.ID1=PhysicalPodAddress;
    messagehandler.ID2=PhysicalPodAddress;
    messagehandler.SetMessageSequence(MessageSeqStart);
    messagehandler.packethandler.Sequence=PacketSeqStart;
    messagehandler.SetLotTid(mlot,mtid);
    if(Monitoring)
    {
        while(1)
        {
           messagehandler.WaitForNextMessage();
                  
        }
    }
    else
    {
      
       if(messagehandler.GetPodState(0x46)==-1) {fprintf(stderr,"Get State");exit(0);};
      
       //messagehandler.Bolus(0.1);
        //sleep(20);
        //messagehandler.GetPodState(0x46);  
        // **************  THE COMPLETE PAIRING PROCESS *****************
        
        //messagehandler.ID1=0xFFFFFFFF;
        //messagehandler.ID2=0xFFFFFFFF;
        
        
       /* while(messagehandler.Pairing(PhysicalPodAddress)==-1)
        {
              
       
        }
        
        //if(messagehandler.VerifyPairing(PhysicalPodAddress)==-1)  {fprintf(stderr,"VerifyPairing Failed");exit(0);}   //Lot ID seems not correct : fixme !  
        if(messagehandler.GetPodState(0x0)==-1) {fprintf(stderr,"Get State");exit(0);};

       // if(messagehandler.FinishPairing(PhysicalPodAddress)==-1) {fprintf(stderr,"FinishPairing Failed");exit(0);} ;   
        if(messagehandler.FinishPairing2(PhysicalPodAddress)==-1) {fprintf(stderr,"FinishPairing2 Failed");exit(0);} ;
        sleep(1);
        if(messagehandler.Purging()==-1) {fprintf(stderr,"Purging Failed");exit(0);}  
        sleep(60);
       
         if(messagehandler.FinishPurging()==-1) {fprintf(stderr,"FinishPurging Failed");exit(0);} 
        //Ready for injection
        if(messagehandler.BeginInjection()==-1) {fprintf(stderr,"BeginInjection Failed");exit(0);}    
        sleep(30);
    
        if(messagehandler.FinishInjection()==-1) {fprintf(stderr,"FinishInjection Failed");exit(0);}
          if(messagehandler.FinishInjection2()==-1) {fprintf(stderr,"FinishInjection Failed");exit(0);}
      sleep(5);
        messagehandler.GetPodState(0);
         */      
        while(1)
        {
           messagehandler.WaitForNextMessage();
                  
        } 
       
    };
    
}
