#include<reg51.h>
//#include"lcd.h"

void main()
	{
	unsigned char i;
	TMOD=0x20;
	SCON=0x50;
	TH1=0xfd;
	TR1=1;
		
	while(RI==0);
	RI=0;
	i=SBUF;
	P2=i;
	}
	