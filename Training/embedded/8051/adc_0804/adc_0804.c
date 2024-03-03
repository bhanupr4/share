#include<reg51.h>

sbit RD1=P3^0;

sbit WR1=P3^1;

sbit INTR1=P3^2;	 

void read();
void write();
void delay();


 

void main()
	{
	P3=0x04;

//	   while(1)
//	   	{
		write();
		while(INTR1==0);
		read();
		delay();
//		}
	}

void delay()
	{
	 unsigned int i;
	 for(i=0;i<20000;i++);
	}


void read()
	{
	 RD1=1;
	 delay();
	 RD1=0;
	}

void write()
	{
	 WR1=0;
	 delay();
	 WR1=1;
	}
