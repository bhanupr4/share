#include<reg51.h>

void delay()
	{
	unsigned int i;
	for(i=0;i<40000;i++);
	}
	
void main()
	{
	unsigned char i;
	TMOD=0x20;   
	SCON=0x50;
	TH1=0xfd;    // 9600 in baud rate
	TR1=1;
	SBUF=0x50;
	while(TI==0);    // polling 
	TI=0;
	delay();
	while(1);
	}