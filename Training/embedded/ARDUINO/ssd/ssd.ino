const int ar[]={7,6,5,4,3,2,1};

void setup() {
for(int i=0;i<7;i++)
  {
  pinMode(ar[i],OUTPUT);
  }
}

void loop() {
      
      digitalWrite(ar[0],HIGH);
      digitalWrite(ar[1],HIGH);
      digitalWrite(ar[2],HIGH);
      digitalWrite(ar[3],HIGH);
      digitalWrite(ar[4],HIGH);
      digitalWrite(ar[5],HIGH);
      digitalWrite(ar[6],LOW);
     
      delay(500);    // 1s delay
      
      digitalWrite(ar[0],LOW);
      digitalWrite(ar[1],HIGH);
      digitalWrite(ar[2],HIGH);
      digitalWrite(ar[3],LOW);
      digitalWrite(ar[4],LOW);
      digitalWrite(ar[5],LOW);
      digitalWrite(ar[6],LOW);

        delay(500);
        digitalWrite(ar[0],HIGH);
      digitalWrite(ar[1],HIGH);
      digitalWrite(ar[2],LOW);
      digitalWrite(ar[3],HIGH);
      digitalWrite(ar[4],HIGH);
      digitalWrite(ar[5],LOW);
      digitalWrite(ar[6],HIGH);
      delay(500);
      digitalWrite(ar[0],HIGH);
      digitalWrite(ar[1],HIGH);
      digitalWrite(ar[2],HIGH);
      digitalWrite(ar[3],HIGH);
      digitalWrite(ar[4],LOW);
      digitalWrite(ar[5],LOW);
      digitalWrite(ar[6],HIGH);
      delay(500);
      digitalWrite(ar[0],LOW);
      digitalWrite(ar[1],HIGH);
      digitalWrite(ar[2],HIGH);
      digitalWrite(ar[3],LOW);
      digitalWrite(ar[4],LOW);
      digitalWrite(ar[5],HIGH);
      digitalWrite(ar[6],HIGH);
      delay(500);
      digitalWrite(ar[0],HIGH);
      digitalWrite(ar[1],LOW);
      digitalWrite(ar[2],HIGH);
      digitalWrite(ar[3],HIGH);
      digitalWrite(ar[4],LOW);
      digitalWrite(ar[5],HIGH);
      digitalWrite(ar[6],HIGH);
      delay(500);
      digitalWrite(ar[0],HIGH);
      digitalWrite(ar[1],LOW);
      digitalWrite(ar[2],HIGH);
      digitalWrite(ar[3],HIGH);
      digitalWrite(ar[4],HIGH);
      digitalWrite(ar[5],HIGH);
      digitalWrite(ar[6],HIGH);
      delay(500);
      digitalWrite(ar[0],HIGH);
      digitalWrite(ar[1],HIGH);
      digitalWrite(ar[2],HIGH);
      digitalWrite(ar[3],LOW);
      digitalWrite(ar[4],LOW);
      digitalWrite(ar[5],LOW);
      digitalWrite(ar[6],LOW);
      delay(500);
      digitalWrite(ar[0],HIGH);
      digitalWrite(ar[1],HIGH);
      digitalWrite(ar[2],HIGH);
      digitalWrite(ar[3],HIGH);
      digitalWrite(ar[4],HIGH);
      digitalWrite(ar[5],HIGH);
      digitalWrite(ar[6],HIGH);
      delay(500);
      digitalWrite(ar[0],HIGH);
      digitalWrite(ar[1],HIGH);
      digitalWrite(ar[2],HIGH);
      digitalWrite(ar[3],LOW);
      digitalWrite(ar[4],LOW);
      digitalWrite(ar[5],HIGH);
      digitalWrite(ar[6],HIGH);
      delay(500);
}
  
