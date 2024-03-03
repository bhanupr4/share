#include<TimerOne.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(42, OUTPUT);//R M
  analogWrite(42, 150);
  delay(150);
  pinMode(43, OUTPUT);//G U
  digitalWrite(43, HIGH);
  delay(50);
  analogWrite(42, 0);
  delay(150);
  pinMode(47, OUTPUT);//Y S
  analogWrite(47, 150);
  delay(50);
  digitalWrite(43, LOW);
  delay(150);
  analogWrite(47, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  pinMode(42, OUTPUT);//R M
  digitalWrite(42, 1);
  delay(150);

  pinMode(43, OUTPUT);//G U
  digitalWrite(43, HIGH);
  delay(50);
  digitalWrite(42, 0);
  delay(150);

  pinMode(47, OUTPUT);//Y S
  digitalWrite(47, 1);
  delay(50);
  digitalWrite(43, LOW);
  delay(150);
  digitalWrite(47, 0);
  delay(150);
  digitalWrite(42, 1);
  delay(15);
  digitalWrite(47, 1);
  delay(15);
  digitalWrite(43, HIGH);
  delay(400);
  digitalWrite(42, 0);
  digitalWrite(47, 0);
  digitalWrite(43, 0);
  delay(500);
}
