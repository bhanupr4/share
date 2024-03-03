#include<reg51.h>

void delay();

void delay()
	{
		unsigned int i;
		for(i=0;i<20000;i++);
	}
	
void main()
	{
		unsigned char i,ar[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
		P2=0x00;
		while(1)
			{
				for(i=0;i<10;i++)
					{
						P1= ar[i];
			//			delay();
						delay();
					//	delay();
					}
				}
		}
	