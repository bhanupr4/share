#include<reg51.h>

void delay(unsigned int);

void delay(unsigned int j)
	{
		unsigned int i,k;
		for(i=0;i<j;i++)
			{
				for(k=0;k<1275;k++);
			}
	}
void main()
	{
		int i;
		P2=0xff;
		while(1)
		{
			for(i=0;i<10;i++)
				{
					P2=i;
					delay(1000);
				}
		}
	}
		