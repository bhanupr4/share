#include<reg51.h>

sbit rs=P3^0;
sbit en=P3^1;

void cmd(unsigned char);   // command function
void dat(unsigned char);   // data function
void delay();              //  delay function
void printi(unsigned int);  // function for passing numbers
void print(unsigned char *); //  function for passing string
void inti();

void delay()
	{
	unsigned int i;
	for(i=0;i<20000;i++);
	}

void inti()
	{
	 cmd(0x01);   // clear screen
	 cmd(0x38);   // 8-bit mode
//	 cmd(0x28);   // 4-bit mode
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
//	int count=0;

	 while(*a!='\0')
	 	{
		dat((*a));
		a++;
		}
	}

void main()
	{ 
	int i;
	
	P2=0X00;
	inti();
	while(1)
		{
		cmd(0x80);  // 8 indicates 0th row and 0 indicates 0th column  
//		dat('I');	
       
			print("INDIA");
		cmd(0xc0);
		printi(1234);			
		while(1);
		}
	}
	 