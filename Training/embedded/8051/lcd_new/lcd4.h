#include<reg51.h>

sbit rs=P1^0;
sbit en=P1^1;

void cmd(unsigned char);
void dat(unsigned char);
void delay();
void printi(unsigned int);
void print(unsigned char *);

void delay()
	{
	unsigned int i;
	for(i=0;i<20000;i++);
	}

void inti()
	{
	 cmd(0x01); // clear screen
	 cmd(0x38); // 8-bit mode
	 cmd(0x06); //
	 cmd(0x0e);
	}

void cmd(unsigned char cd)
	{
	P2=cd;
	rs=0;   // command
	en=0;
	delay();
	en=1;
	}

void dat(unsigned char da)
	{
	P2=da;
	rs=1;  // data
	en=0;
	delay();
	en=1;
	}

void printi(unsigned int a)
	{
	 unsigned int b=a,d=1;
	  while(a>9)
	  	{
		 a=a/10;
		 d=d*10;
		}

	 while(b>0)
	  	{
		dat(((b/d)+48));
		b=b%d;
		d=d/10;
		}
	 }

void print(unsigned char *a)
	{
	 while(*a!='\0')
	 	{
		dat((*a));
//		count++;
		a++;
		}
	}
	

