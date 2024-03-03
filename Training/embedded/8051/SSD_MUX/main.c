#include<reg51.h>

//sbit seg1=P1^0;
//sbit seg2=P1^1;
//sbit seg3=P1^2;

void delay();

void delay()
	{
		unsigned int i;
		for(i=0;i<10000;i++);
	}
	
void main()
	{
		unsigned int i,j,ar[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
		P2=0x00;
		
		while(1)
			{
				for(i=0;i<1000;i++)
					{
/*						seg1=0;
						seg2=1;
						seg3=1;*/
						P1=~0x04;
						P2= ar[i/100];  //0
						delay();
						j=i%100;
						P1=~0x02;
						P2= ar[j/10];  //0
						delay();
//						delay();
						P1=~0x01;
						P2= ar[j%10];  //
						delay();
					}
				}
		}
	