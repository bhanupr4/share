#include<reg51.h>

void delay(unsigned int);      // fxn prototype

void delay(unsigned int j)       // fxn def
	{
		unsigned int i,k;
		for(i=0;i<j;i++)
				{
					for(k=0;k<1000;k++);
				}		
	}

void main()
	{
		P1=0xff;    // 
		while(1)
			{
				P1=~P1;
				delay(1000);    // fxn call
//				P1=0x00;
//				delay(1);    // fxn call
			}
		}
			
	