#define MAX_BYTE_BODY (25)
enum {ACK=0b010,CON=0b100,PDM=0b101,POD=0b111}; 
class Packet
{

private:
int SetPODPDMFromFrame(unsigned char *Frame,int Len);
int SetACKFromFrame(unsigned char *Frame,int Len);
int SetCONFromFrame(unsigned char *Frame,int Len);
public:
unsigned int ID1=0;
unsigned char Type=0;
unsigned char Sequence=0;
unsigned int PacketLen=0;
unsigned char crc8=0;
unsigned char Body[MAX_BYTE_BODY];
bool IsValid=false;

int SetPacketFromFrame(unsigned char *Frame,int Len);
Packet();
Packet(unsigned char *Frame,int Len);
~Packet();
unsigned char computecrc_8(unsigned char crc, const void *data, int data_len);
void PrintState();
int GetFrame(unsigned char *Frame);
};
