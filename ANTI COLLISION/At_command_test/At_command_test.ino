//NL & CR
#include<SoftwareSerial.h>
SoftwareSerial wifi(7, 8);//Rx,Tx
void setup() {
  wifi.begin(9600);
  Serial.begin(9600);
  wifi.println("AT");
  Serial.println(wifi.readString());
}

void loop() {
  // put your main code here, to run repeatedly:
  while (wifi.available())
  {
    Serial.println("reading ......");
    String st = "";
    st = wifi.readString();
    /*for (int i = 0; st[i] != NULL; i++){
      Serial.print("st[");
      Serial.print(i);
      Serial.print("]= ");
      Serial.println(st[i]);
      }*/
    Serial.print(st);
    Serial.println();
    st = "";
  }
  while (Serial.available()) {
    Serial.println("writing...");
    String st1 = "";
    st1 = Serial.readString();
    Serial.println(st1);
    wifi.println(st1);
    st1 = "";
  }
}
