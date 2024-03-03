
const int p1=2,p2=3,p3=4,p4=5;
boolean a=LOW;
boolean b=LOW;
boolean c=LOW;
boolean d=LOW;
int e;

void setup() {

  pinMode(p1,INPUT);
  pinMode(p2,INPUT);
  pinMode(p3,INPUT);
  pinMode(p4,INPUT);
  Serial.begin(9600);
}

void loop() {
a=digitalRead(p1);
b=digitalRead(p2);
c=digitalRead(p3);
d=digitalRead(p4);

e=8*d+4*c+2*b+1*a;
Serial.println(e);
delay(1000);



}
