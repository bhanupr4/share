#include<reg51.h>

void delay()
	{
	unsigned int i;
	for(i=0;i<30000;i++);
	}

void main()
	{
	P2=0X00;    // SETS OUTPUT
		while(1)
			{
			P2=0X01;
			delay();
			delay();
			P2=0X02;
			delay();
			delay();
			P2=0X04;
			delay();
			delay();
			P2=0X08;
			delay();
			delay();
//			while(1);
			}
	}
