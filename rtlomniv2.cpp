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
        //messagehandler.GetPodState(0x46); 
        //messagehandler.GetPodState(0); 
        //sleep(10);
        messagehandler.Bolus(0.1);    
        //messagehandler.GetPodState(0x46);
        //sleep(5);
         //messagehandler.GetPodState(0); 
        //sleep(5);
        //messagehandler.Bolus(0.1);    
        // **************  THE COMPLETE PAIRING PROCESS *****************
        /*
        if(messagehandler.Pairing(0xF50E0)==-1)
        {
              messagehandler.packethandler.Sequence=PacketSeqStart+1;
              messagehandler.Lotid=12345;
              messagehandler.Tid=78910;  
       
        }
        if(messagehandler.VerifyPairing(0xF50E0)==-1)  messagehandler.packethandler.Sequence=PacketSeqStart+2;     
        if(messagehandler.FinishPairing(0xF50E0)==-1) messagehandler.packethandler.Sequence=PacketSeqStart+3;   
        if(messagehandler.FinishPairing2(0xF50E0)==-1) messagehandler.packethandler.Sequence=PacketSeqStart+4;
        if(messagehandler.Purging()==-1) messagehandler.packethandler.Sequence=PacketSeqStart+5;
        sleep(60);
        //TempBasal Command 1a->1a->c8720bfc0001dc222cc000031004b803f804f8041804#10
        //CanceTime(4)    : 	Command 19->19->417b1a9879a410df0502280012830602020f00000202 
        // Command 1a->1a->e3ca66ed020065010050000a000a#14 PDM 1a(14)->Insulin Schedule:Nonce:e3ca66ed(6) Type:02 Bolus CheckSum:0065/0065 Duration:01(30 minutes) FiledA:0050(0.25U) UnitRate:000a(0.50U) UnitRateSchedule:000a(0.50U) 
(2:1)	Command 17->17->000064000186a0000000000000#14 PDM 17(13)->InsulinExtra(short):Immediate 0.50U
        //messagehandler.GetPodState(0);
        if(messagehandler.FinishPurging()==-1) messagehandler.packethandler.Sequence=PacketSeqStart+6;
        */

        while(1)
        {
           messagehandler.WaitForNextMessage();
                  
        } 
       
    };
    
}
