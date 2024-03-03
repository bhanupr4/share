/* DOCUMENTATION FOR STEPPER MOTOR MITSUMI 
STEP ANGLE IS 7.5 DEGREE 
MEANS IN SINGLE SEQUENCE IT WILL ROTATE 7.5 DEGREE
*/



const int black=2,brown=3,orange=4,yellow=5;

int delay1=100;

void setup() {
pinMode(black,OUTPUT);
pinMode(brown,OUTPUT);
pinMode(orange,OUTPUT);
pinMode(yellow,OUTPUT);
/*
for(int i=0;i<3;i++)
{
digitalWrite(black,HIGH);
digitalWrite(brown,LOW);
digitalWrite(orange,LOW);
digitalWrite(yellow,HIGH);
delay(delay1);

digitalWrite(black,HIGH);
digitalWrite(brown,LOW);
digitalWrite(orange,HIGH);
digitalWrite(yellow,LOW);
delay(delay1);

digitalWrite(black,LOW);
digitalWrite(brown,HIGH);
digitalWrite(orange,HIGH);
digitalWrite(yellow,LOW);
delay(delay1);

digitalWrite(black,LOW);
digitalWrite(brown,HIGH);
digitalWrite(orange,LOW);
digitalWrite(yellow,HIGH);
delay(delay1);
}
*/
}

void loop() {
digitalWrite(black,HIGH);
digitalWrite(brown,LOW);
digitalWrite(orange,LOW);
digitalWrite(yellow,HIGH);
delay(delay1);

digitalWrite(black,HIGH);
digitalWrite(brown,LOW);
digitalWrite(orange,HIGH);
digitalWrite(yellow,LOW);
delay(delay1);

digitalWrite(black,LOW);
digitalWrite(brown,HIGH);
digitalWrite(orange,HIGH);
digitalWrite(yellow,LOW);
delay(delay1);

digitalWrite(black,LOW);
digitalWrite(brown,HIGH);
digitalWrite(orange,LOW);
digitalWrite(yellow,HIGH);
delay(delay1);
}
