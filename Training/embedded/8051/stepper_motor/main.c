#include<reg51.h>

void delay(unsigned int);

void delay(unsigned int i)
	{
		unsigned int y,z;
		for(y=0;y<i;y++)
			{
			for(z=0;z<1375;z++);
			}
	}
	
void main()
	{
		P2=0x00;   // sets as output
		while(1)
			{
				P2=0x06;
				delay(100);
				P2=0x05;
				delay(100);
				P2=0x0a;
				delay(100);
				P2=0x09;
				delay(100);
			}
	}
	
