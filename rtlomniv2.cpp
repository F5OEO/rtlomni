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
        /*case 'l': // Lot
			mlot=atol(optarg);
			break;
        case 't': // Tid
			 mtid=atol(optarg);
			break; 
        case 'c': // Colorize message
			 colorize=1;
			break;    
         case 'd': // Debug mode : recording I/Q
			 debugfileiq=1;
			break;      
        case 'm': // Mode Tx ou Rx
            if(strcmp(optarg,"tx")==0) ModeTx=1;        
			break;     
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
        */    
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
    Modem.SetIQFile(outputname,1);
    unsigned char Frame[MAX_BYTE_PER_PACKET];
    unsigned char FrameTx[MAX_BYTE_PER_PACKET];
    for(int i=0;i<MAX_BYTE_PER_PACKET;i++) FrameTx[i]=i; 
    while(1)
    {
        printf("Receive\n"); 
        //for(int i=0;i<10;i++)
        {   
            int Len=Modem.Receive(Frame,400);
            
            if(Len<=0)
            {
                printf(".");fflush(stdout);
            }
            else
            {
                for(int i=0;i<Len;i++) printf("%02x",Frame[i]);
                printf("\n");
            }
        }
        usleep(100000);
        printf("Idle\n");
        Modem.Transmit(FrameTx,MAX_BYTE_PER_PACKET);
        
    }
    
}
