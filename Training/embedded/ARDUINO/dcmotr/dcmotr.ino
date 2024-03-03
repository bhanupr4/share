
const int pin1=7,pin2=6,sw=8,en1=5;

boolean flag=HIGH;

void setup() {
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(en1,OUTPUT);
  pinMode(sw,INPUT);  
}

void loop() {
  
  flag=digitalRead(sw);

  if(flag==LOW)
    {  
    digitalWrite(pin1,HIGH);
    digitalWrite(pin2,LOW);
    digitalWrite(en1,HIGH);
    delay(5000);
    }
    
   else
    {  
    digitalWrite(en1,LOW);
    }
}
