
int led=9;
int val=0;
int a;

void setup() {
  pinMode(led,OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  val=analogRead(A1);
  a=val/4;
  
  analogWrite(led,a);
  // put your main code here, to run repeatedly:

}
