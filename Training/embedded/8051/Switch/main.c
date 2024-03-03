#include<reg51.h>

sbit sw=P0^0;

void delay(unsigned int);

void delay(unsigned int i)	
	{
		int j,k;
		for(j=0;j<i;j++)
			{
				for(k=0;k<1275;k++);
			}
	}
	
void main()
	{
		P1=0x00;
			while(1)   // 
			{
				if(sw==0)
				{					
					while(sw==0);
					P1=~P1;
					delay(400);
				}
/*				else
				{					
					P1=0x00;				
//				delay(40);
				}*/
		}
	}
	