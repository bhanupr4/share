const int led=13,sw=9;

boolean flag=HIGH;

void setup() {
pinMode(led,OUTPUT);
pinMode(sw, INPUT);
}

void loop() {
  
flag=digitalRead(sw);

  while(!flag)
  {
    digitalWrite(led,HIGH);
///    delay(1000);
flag=digitalRead(sw);
  }  

//  else
    digitalWrite(led,LOW);

  // put your main code here, to run repeatedly:

}
