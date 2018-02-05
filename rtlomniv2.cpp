#include "RFModem.h"
#include <getopt.h>


#define PROGRAM_VERSION "0.0.2"

void print_usage()
{

	fprintf(stderr,\
"rtlomni -%s\n\
Usage:rtlomni -i File [-l LotID] [-t Tid][-c][-m Mode][-a Address][-n Msg sequence][-p PacketSeq][-h] \n\
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
    unsigned char Frame[MAX_BYTE_PER_PACKET];
    while(1)
    {
        int Len=Modem.Receive(Frame,400);
        if(Len<=6)
        {
            printf(".");fflush(stdout);
        }
        else
            printf("\n Frame %d\n",Len);
    }
    
}
