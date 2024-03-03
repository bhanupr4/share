#include<reg51.h>

sbit rs=P3^0;
sbit en=P3^1;

void cmd(unsigned char);
void dat(unsigned char);
void delay();

void delay()
	{
	unsigned int i;
	for(i=0;i<20000;i++);
	}

void inti()
	{
	 cmd(0x01);
	 cmd(0x38);
	 cmd(0x06);
	 cmd(0x0e);
	}

void cmd(unsigned char cd)
	{
	P2=cd;
	rs=0;
	en=0;
	delay();
	en=1;
	}

void dat(unsigned char da)
	{
	P2=da;
	rs=1;
	en=0;
	delay();
	en=1;
	}


void main()
	{
	P2=0X00;
	inti();
	while(1)
		{
		cmd(0x80);
		dat(48);
/*		dat(73);
		dat(78);
		dat(68);
		dat(73);
		dat(65);*/
		while(1);
		}
	}
	 