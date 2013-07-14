#include "xbee.h"

SerialStream firacom;
void UART_init()
{
char p=0;
	firacom.Open("/dev/ttyUSB0");
	firacom.SetBaudRate(SerialStreamBuf::BAUD_57600 );
	  while ( ! firacom.good() )
     {
         std::cerr << "Error: Could not set the baud rate.\n";
         firacom.SetBaudRate(SerialStreamBuf::BAUD_57600 );
     }
	firacom.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 );
	firacom.SetParity(SerialStreamBuf::PARITY_NONE);
	firacom.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE);
	firacom.SetNumOfStopBits(1);
while(p !='M')
{
firacom>>p;
}
}

int send_command(char phrase[4])
//int main()
{
	
	char p=0;
	//UART_init();

		firacom<<phrase[0];
    	firacom<<phrase[1];
		firacom<<phrase[2];
		firacom<<phrase[3];
		
	p=0;
	return 0;
}

