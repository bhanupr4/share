
int a;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {

if(Serial.available()>0)
{
a=Serial.read();
a=a-'0';
Serial.println(a);


delay(1000);
}
}
