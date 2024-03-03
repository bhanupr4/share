/*
   String sttemp = wifi.readString();
  Serial.println(sttemp);
  for (int i = 0; sttemp[i] != NULL; i++)
  {
    Serial.print("st[");
    Serial.println(i);
    Serial.println("]=");
    Serial.println(sttemp[i]);
  }
*/



#include<SoftwareSerial.h>
SoftwareSerial wifi(7, 8);//Rx,Tx
boolean conected = false;
int xi, yi;
char output[512];


void setup() {
  // put your setup code here, to run once:
  wifi.begin(9600);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.println("Callibrating hold it for a beep or 2 second......");
  delay(1000);
  xi = analogRead(A0);
  yi = analogRead(A1);
  Serial.println("Calibrated");
  sprintf(output, "\tX= %d\t\t\t\t Y= %d\t\t\t\t", xi, yi);
  Serial.println(output);

  wifi.println("AT+RST");
  Serial.println(wifi.readString());

  wifi.println("AT");
  Serial.println(wifi.readString());
conectagain:
  wifi.println("AT+CWQAP");
  Serial.println(wifi.readString());

  wifi.println("AT+CWMODE=1");
  Serial.println(wifi.readString());
  wifi.println("AT+CWJAP=\"DEVICE1\",\"12345678\"");
  Serial.println(wifi.readString());
  while (!wifi.available())
  { Serial.print(".");
    delay(10);
  }
  delay(100);
  wifi.println("AT+CIPMUX=0");
  Serial.println(wifi.readString());

  wifi.println("AT+CIPSTART=\"UDP\",\"192.168.4.1\",53");//I=39,O  I=40,K  || I=37,A I=38,L
  String sttemp = wifi.readString();
  Serial.println(sttemp);
  for (int i = 35; sttemp[i] != NULL; i++)
  {
    Serial.println("Connecting");
    if (sttemp[i] == 'A')
    {
      if (sttemp[i + 1] == 'L')
      {
        if (sttemp[i + 2] == 'R')
        {
          conected = true;
          goto conected;
        }
      }
    }

    if (sttemp[i] == 'O')
    {
      if (sttemp[i + 1] == 'K')
      {
        conected = true;
        goto conected;
      }
    }
  }
  Serial.println("not connectected RETRYING ......");
  conected = false;
  goto conectagain;
conected:
  if (conected == true)
    Serial.println("Connected NOW WE CAN SEND DATA");
}

void loop() {
  // put your main code here, to run repeatedly:
  //sprintf(output, "\tX= %d\t\t\t\t Y= %d\t\t\t\t", (analogRead(A0) - xi), (analogRead(A1) - yi));
  //Serial.println(output);
  //delay(100);
  Serial.println("sending");
  if (conected == true)
  {
    int x = (analogRead(A0) - xi);
    int y = (analogRead(A1) - yi);
    int len = 0;//for length of string
    String stsend = "X";
    stsend += x;
    stsend = stsend + "Y";
    stsend += y;
    stsend = stsend + "E";//TO TERMINATE STRING
    //calculating length of string
    for (int i = 0; stsend[i] != NULL; i++)
    {
      len++;
    }
    Serial.println(stsend);
    //Serial.println(len);
    stsend = "AT+CIPSEND=";
    stsend += len;
    wifi.println(stsend);
    stsend = "X";
    stsend += x;
    stsend = stsend + "Y";
    stsend += y;
    stsend = stsend + "E";
    wifi.println(stsend);
    Serial.println(wifi.readString());
    delay(1000);


  }
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
