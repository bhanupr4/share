const int sw=12;
boolean sw1=LOW;
int ar[]={0,1,2,3,4,5,6,7};

void setup() {

pinMode(sw,INPUT);
for(int i=0;i<8;i++)
{
pinMode(ar[i],OUTPUT);
}
}


void loop()    // infinite loop
{
  sw1=digitalRead(sw);
  
  if (sw1==HIGH)
  {
for(int i=0;i<8;i++)
{
digitalWrite(ar[i],HIGH);
delay(1000);
digitalWrite(ar[i],LOW);
}
  }
//delay(1000);
}
