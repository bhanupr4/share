#include<reg51.h>
 
 void delay(unsigned int);
 
  void delay(unsigned int a)
	{
		int i;
		for(i=0;i<20000;i++);
	}
	
	void main()
	{
		int i;
		P1=0x80;
		delay(45);
		
		while(1)
		{
			for(i=0;i<7;i++)
			{
				P1=P1>>1;
				delay(45);				
			}
			
			if(P1==0x01)
			{
				P1=0x80;
				delay(45);
			}
		}
	}