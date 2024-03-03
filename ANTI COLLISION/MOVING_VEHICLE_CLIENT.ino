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

#define pie 3.142857142857143
#include <LiquidCrystal.h>
#include<SoftwareSerial.h>
SoftwareSerial wifi(7, 8);//Rx,Tx
//LiquidCrystal lcd(12, 11, 2, 3, 4, 5);//RS, E, D4-D7

boolean conected = false;
int xi, yi;
int x, y;
int theta, phie; //theta=self angle / phie=another device
char output[512];
int buzz = 13;
int distance;

int ml = 5;
int gml = 6;
int mr = 9;
int gmr = 10;

int sped = 180;

int led = 12;

int r1 = 6, r2 = 6;

void sending()
{
  Serial.println("sending");
  if (conected == true)
  {
    x = (analogRead(A0) - xi) + 1;
    y = (analogRead(A1) - yi) + 1;
    theta = ((180.00 / pie) * atan(float(y) / float(x))) * 100;
    theta = abs(theta);
    phie = ((180 / pie) * atan(1));
    int len = 0;//for length of string
    String stsend = "X";
    stsend += x;
    stsend = stsend + "Y";
    stsend += y;
    stsend = stsend + "T";
    stsend += theta;
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
    stsend = stsend + "T";
    stsend += theta;
    stsend = stsend + "E";
    wifi.println(stsend);
    Serial.println(wifi.readString());
  }
}
//////// algo
void analyze()
{
  distance = (sqrt(pow((x - 1), 2) + pow((y - 1), 2)));
  Serial.print("distance=");
  Serial.println(distance);
  if (distance <= r1 + r2)
  {
    Serial.print("\t\t UNSAFE\t\t");
    Serial.println();
    digitalWrite(led, HIGH);
    stopp();
    tone(buzz, 2000);
    delay(100);
    noTone(buzz);
    delay(100);

    tone(buzz, 2000);
    delay(100);
    noTone(buzz);
    delay(100);

    tone(buzz, 2000);
    delay(100);
    noTone(buzz);
    delay(100);

    tone(buzz, 2000);
    delay(100);
    noTone(buzz);
    delay(100);
  }
  if (distance > r1 + r2)
  {
    Serial.print("\t\t SAFE\t\t");
    Serial.println();
    movenormal();
    digitalWrite(led, LOW);
    digitalWrite(buzz, LOW);
  }
}

/////locomotion

void forward()
{
  analogWrite(ml, sped);
  analogWrite(gml, 0);
  analogWrite(mr, sped);
  analogWrite(gmr, 0);
}

void rt()
{
  analogWrite(ml, sped);
  analogWrite(gml, 0);
  analogWrite(mr, 0);
  analogWrite(gmr, 0);
}
void lt()
{
  analogWrite(ml, 0);
  analogWrite(gml, 0);
  analogWrite(mr, sped);
  analogWrite(gmr, 0);
}

void movenormal()
{
  forward();
  rt();
  lt();
}
void stopp()
{
  analogWrite(ml, 0);
  analogWrite(gml, 0);
  analogWrite(mr, 0);
  analogWrite(gmr, 0);
}



void setup() {
  // put your setup code here, to run once:
  wifi.begin(9600);
  //lcd.begin(16, 2);
  //lcd.print("hello, world!");
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  pinMode(ml, OUTPUT);
  pinMode(gml, OUTPUT);
  pinMode(mr, OUTPUT);
  pinMode(gmr, OUTPUT);

  pinMode(led, OUTPUT);
  pinMode(buzz, OUTPUT);

  stopp();
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
  digitalWrite(buzz, HIGH);
  delay(1000);
  digitalWrite(buzz, LOW);
  conected = false;
  goto conectagain;
conected:
  if (conected == true)
  { Serial.println("Connected NOW WE CAN SEND DATA");
    digitalWrite(buzz, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
    delay(200);
    digitalWrite(buzz, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //sprintf(output, "\tX= %d\t\t\t\t Y= %d\t\t\t\t", (analogRead(A0) - xi), (analogRead(A1) - yi));
  //Serial.println(output);
  //delay(100);
  sending();
  analyze();
  delay(100);
  /*while (wifi.available())
    {
    Serial.println("reading ......");
    String st = "";
    st = wifi.readString();
    /*for (int i = 0; st[i] != NULL; i++){
      Serial.print("st[");
      Serial.print(i);
      Serial.print("]= ");
      Serial.println(st[i]);
      }
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
    }* /*/
}
