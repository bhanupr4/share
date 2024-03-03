const int sw=12;

boolean SW1=HIGH;

int ar[]={2,3,4,5,6,7,8,9};

void setup() 
{
  pinMode(sw,INPUT);
  for(int j=0;j<8;j++)
    {
    pinMode(ar[j],OUTPUT);
    }
}

void loop(){

SW1=digitalRead(sw);
if(SW1==LOW){
for(int i=0;i<8;i++)
{
  digitalWrite(ar[i],HIGH);
  delay(500);
  digitalWrite(ar[i],LOW);
//  delay(500);

}}}

