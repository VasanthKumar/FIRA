/**************************************************************************************************
		PIN Configuration:
16,18	P 0.0 : PWM 1 for left motor
		P 0.1 : PWM 3 for right motor
		P 0.2-0.5: GPIO for motors
		P 0.8: TX for UART 1
		P 0.14 :Encoder for left motor
		P 0.15 : Encoder for right motor
		
********************************************************************************/ 

#include  <lpc214x.h>		 //Includes LPC2148 register definitions
#include <stdio.h>
 
#define Fosc            12000000                    
#define Fcclk           (Fosc * 5)                  
#define Fcco            (Fcclk * 4)                 
#define Fpclk           (Fcclk ) * 1             

//12M, 60M , 240M, 60M 

#define  UART_BPS	9600	 //Set Baud Rate here


void forward();
void back();
void left();
void right();
void stop();
void pwm(int, int);
void pwm_char(char,char);

void  Init_UART1(void)					//This function setups UART1
{  
   unsigned int Baud16;
   U0LCR = 0x83;		            // DLAB = 1
   Baud16 = (Fpclk / 16) / UART_BPS;  
   U1DLM = Baud16 / 256;							
   U1DLL = Baud16 % 256;						
   U1LCR = 0x03;
}
				

void  UART1_SendByte(unsigned char data)	   //A function to send a byte on UART1
{  
   U1THR = data;				    
   while( (U1LSR&0x40)==0 );	    
}

// Blocking code
char  UART1_ReceiveByte()	   //A function to send a byte on UART1
{  	
		char data;
   while( (U1LSR&0x01)==0 );
		data=U1RBR;
	return data;
}

void pwminit()
{
	PINSEL0=(1<<1)|(1<<3)|(1<<16)|(1<<18);
	PWMTCR=0x02;				//Reset everything
	PWMTCR=0x05;
	PWMTC=0x0;						//Set timer to zero
	PWMPC=0x00;						//Prescaling set to zero
	PWMPR=0x09; 					//Maximum value of timer counter
	PWMPCR=0xA00; 				// Set PWM1 and PWM 3 as output enable
	PWMMR0=0x80;					//Max value to be reached
	PWMMCR=0x02;;					//pwmtc will reset when it reaches 128.(pwmmr0)
	PWMLER=0xA;						//set latch enable on pwm1 & pwm 3 
}
/*	void ExtInt_Init2(void)
{
 
   EXTMODE |= 4;         //Edge sensitive mode on EINT2
   EXTPOLAR = 0;           //Falling Edge Sensitive
   PINSEL0 |= 0x80000000; //Enable EINT2 on P0.15
   VICVectCntl1 = 0x20 | 16; // 16 is index of EINT2
   VICVectAddr1 = (unsigned long) ExtInt_Serve2;
   VICIntEnable |= 1<<16;   //Enable EINT2
 
}
*/


volatile int flag=0;
volatile	char k='P',p[4];
int main()
{  
	int i;	
	VPBDIV=0x01;														// Set PCLK at 12MHz i.e same as clock
	PINSEL0=(1<<1)|(1<<3)|(1<<16)|(1<<18); 	// Enable UART1 Rx and Tx pins &pwm 1 and pwm 3
	PINSEL1 = 0x00000000;
	PINSEL2 = 0x00000000;
	IO0DIR=(1<<2)|(1<<3)|(1<<4)|(1<<5);			// Set motor pins as outputs 
	Init_UART1();                          // Initialize UART1


	pwminit();
	UART1_SendByte('H');
	while(1)
	{
		while(k!='0')
		{
		k=UART1_ReceiveByte();
		
		//UART1_SendByte(k);
		
		}
		if( k =='0')
		{
			p[0]=UART1_ReceiveByte();
				if(p[0]=='F')
				{
					for (i=1;i<3;i++)
					{
						p[i]=UART1_ReceiveByte();
					}
					forward();
					pwm_char(p[1],p[2]);
					UART1_SendByte('D');
				}
				if(p[0]=='B')
				{
					for (i=1;i<3;i++)
					{
						p[i]=UART1_ReceiveByte();
					}
					back();
					pwm_char(p[1],p[2]);
					UART1_SendByte('D');
				}
				if(p[0]=='w')
				{
					forward();
					UART1_SendByte('D');
				}
				if(p[0]=='s')
				{
					stop();
					UART1_SendByte('D');
				}
				if(p[0]=='a')
				{
					left();
					UART1_SendByte('D');
				}
				if(p[0]=='d')
				{
					right();
					UART1_SendByte('D');
				}
			  if(p[0]=='b')
				 {
					 back();
					 UART1_SendByte('D');
				 }
				 if(p[0]=='p')
				{
					p[1]=UART1_ReceiveByte();
					pwm_char(p[1],p[1]);
					UART1_SendByte('D');
				}
		}
		
			
		}
		
}
		


void forward()
{
	IO0SET=(1<<2)|(1<<4);
	IO0CLR=(1<<3)|(1<<5);
}
void left()
{
	IO0SET=(1<<3)|(1<<4);
	IO0CLR=(1<<2)|(1<<5);
}
void right()
{
	IO0SET=(1<<2)|(1<<5);
	IO0CLR=(1<<3)|(1<<4);
}
void back()
{
	IO0CLR=(1<<2)|(1<<4);
	IO0SET=(1<<3)|(1<<5);
}
void stop()
{
	IO0CLR=(1<<2)|(1<<4)|(1<<3)|(1<<5);
	
}
void pwm(int p,int q)												//give left motor and right motor values between 0-128
{
	PWMMR1=p;
	PWMMR3=q;
	PWMLER=(1<<2)|(1<<4);
	k='P';
}
void pwm_char(char p,char q)												//give left motor and right motor values between 0-128
{
	UART1_SendByte(p);
	UART1_SendByte(q);
	PWMMR1= (int)p;
	PWMMR3= (int)q;
	PWMLER=(1<<2)|(1<<4);
	k='P';
}
