const int out[]={0,1,2,3,4,5,6,7};

unsigned char k=0;

void setup() {

for(int i=0;i<8;i++)
{
  pinMode(out[i],OUTPUT);
}
}

void loop() {
for(int j=2;j<8;j++)
{
 digitalWrite(out[j],HIGH);
}

delay(500);

for(int j=2;j<8;j++)
{
 digitalWrite(out[j],LOW);
}

for(int j=5;j<7;j++)
{
 digitalWrite(out[j],HIGH);
}

delay(500);

}


