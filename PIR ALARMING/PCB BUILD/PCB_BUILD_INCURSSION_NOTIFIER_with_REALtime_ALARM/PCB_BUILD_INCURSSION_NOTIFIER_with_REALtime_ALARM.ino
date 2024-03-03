/*
  AT+CMGF=1
  AT+CMGS="7903461334"
*/

//#define DTR A1//7
#define PIR 2
#define BUZZ 3

//GSM
//#define GSM Serial//sim800l_test//Serial2
#define gsmbaud 19200//9600
#define tout1 100
#define DEBUG       0b00110001 //on error quick get error
//#define error       0b00110000
#define nZF1Eo1     0b10010001
#define ZF1Eo1      0b00010001
#define nZF1E0      0b10010000
#define ZF3oEo1     0b00111001
#define ZF3Eo1      0b00110001
#define ZF3oE0      0b00111000
#define ZF3E0       0b00110000
#define nZF3oEo1    0b10111001
#define nZF3Eo1     0b10110001
#define nZF3oE0     0b10111000
#define nZF3E0      0b10110000
#define GSMtest     0b00001111
#define GPRStest    0b00001000
#define SIMtest     0b00000010
#define NWtest      0b00000100
#define CCLKtest    0b00010000
#define HouUnix     0b00111100
#define MinUnix     0b00111110
#define SecUnix     0b00111111
#define SAP1APN     0b00000010
#define CSTTAPN     0b00000100
#define BOTHAPN     0b00000110
#define oSAP1APN    0b00000011
#define oCSTTAPN    0b00000101
#define oBOTHAPN    0b00000111
#define GSMHeal     0b00000011//at + sim
#define WiFiHeal    0b00000001//at
#define SwRST       0b00000000
#define SigPct      0b00000000
#define Sigusdb     0b00000001
#define Sigsdb      0b10000001

#define RX 16
#define TX 17

#define DEADLINE 10000/10

#define DETECTED HIGH
#define nDETECTED LOW

#define BUZZON LOW
#define BUZZOFF HIGH

const uint8_t rst = 18;

volatile uint16_t count = 0;

uint32_t timer = 0;

#include<TimerOne.h>
#include<SoftwareSerial.h>
#include<BhanuGSM.h>

SoftwareSerial sim800l(RX, TX);//(RX,TX)

BhanuGSM GSM(sim800l);//type-Auto-0 OR GSM-1 OR ESP-2 OR ETHERNET-3 OR LORA-4 etc

void ISR1() {
  //  Serial.println(digitalRead(DTR));
  //  analogRead(PIR) > 400 ? analogWrite(BUZZ, 2) : analogWrite(BUZZ, 0);

  //1SECOND = 10 COUNT
  if (count >= (DEADLINE / 10)) {
    digitalWrite(BUZZ, BUZZON);
  }

  if (digitalRead(PIR) == DETECTED) {
    count++;
  }

  if (digitalRead(PIR) == nDETECTED) {
    digitalWrite(BUZZ, BUZZOFF);
    count = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  //  pinMode(DTR, INPUT_PULLUP);
  pinMode(PIR, INPUT);//_PULLUP);
  pinMode(BUZZ, OUTPUT);
  digitalWrite(BUZZ, HIGH);
  Timer1.initialize(100000);//100ms
  Timer1.attachInterrupt(ISR1);
  Serial.begin(115200);
  sim800l.begin(gsmbaud);
  //  sim800l.setTimeout(100);
  Serial.setTimeout(200);
  //  delay(2000);
  //  sim800l.println(F("AT"));
  //  Serial.println(sim800l.readString());
  GSM.Heal(rst);
  Serial.print(F("N/w "));
  if (!GSM.Test(NWtest))
    Serial.println(F("FAIL"));
  else {
    Serial.println(F("OK"));
    //    GSM.SetAPN(F("www"));
    //    Serial.print(F("GPRS "));
    //    if (!GSM.Test(GPRStest))
    //      Serial.println(F("FAIL"));
    //    else
    //      Serial.println(F("OK"));
  }
  Serial.print(GSM.GetSignal(SigPct));
  Serial.println(F("%"));
  //  SMSreply();
  timer = millis() / 10;
}

//7903461334-BHANU
//9560566599-SUMIT BHAIA
//7979980112-SATYAM
//8116175026-RAJNISH

void SMSreply()
{ Timer1.detachInterrupt();
  GSM.CheckCmd(F("AT+CMGF=1"), F("OK"), ZF3Eo1, tout1);
  if (GSM.CheckCmd(F("AT+CMGS=\"9560566599\""), F(">"), ZF3Eo1, tout1 + 100)) {
    //    GSM.println(F("Warning:- SOME INCURSSION DETECTED INSIDE CHAMBER FOR LAST 10 SECOND\nThis is Auto Generated Message by BHANU INCURSION DETECTOR"));
    GSM.println(F("Alert: A SECURITY BREACH HAS BEEN DETECTED BY DEVICE-1"));
    GSM.TermSend(0x03);
  }
  Timer1.attachInterrupt(ISR1);
}

void 
{
  if (GSM.available() > 0)
  { Serial.println(F("Reading.."));
    String st = GSM.ReadUpto(F("OK"), 3, 2);
    Serial.println(CheckString(&st, F("OK"), 0));
    Serial.println(st);
    //    for (uint16_t i = 0; i < 49; i++)
    //    { Serial.print(F("["));
    //      Serial.print(i);
    //      Serial.print(F("]="));
    //      Serial.println((byte)st[i], HEX);
    //    }
    //    ntp = st;//temporary
    //    Serial.println(ntp);
  }
  if (Serial.available() > 0)
  { String st;
    st = Serial.readString();
    if (CheckString(&st, F("0x"), 2) || CheckString(&st, F("0X"), 2)) {
      uint8_t index = CheckString(&st, F("0"), 1);
      index += 2;
      uint8_t d = 0;
      //      Serial.println(st[index]);
      if ((byte)st[index] >= 48 && (byte)st[index] <= 57) {
        d |= (byte)(st[index] - 48);
      }
      else if ((byte)st[index] >= 65 && (byte)st[index] <= 70) {
        d |= (byte)(st[index] - 55);
      }
      else if ((byte)st[index] >= 97 && (byte)st[index] <= 102) {
        d |= (byte)(st[index] - 87);
      }
      d = d << 4;
      index++;
      //      Serial.println(st[index]);
      //      Serial.println(d, HEX);
      if ((byte)st[index] >= 48 && (byte)st[index] <= 57) {
        d |= (byte)(st[index] - 48);
      }
      else if ((byte)st[index] >= 65 && (byte)st[index] <= 70) {
        d |= (byte)(st[index] - 55);
      }
      else if ((byte)st[index] >= 97 && (byte)st[index] <= 102) {
        d |= (byte)(st[index] - 87);
      }
      //      Serial.println(d, HEX);
      GSM.write(d);
    }
    else {
      Serial.println(st);
      //      GSM.println(st);
      st = GSM.GetCmd(&st, F("OK"), ZF1Eo1, 5000);
      Serial.println(st);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  GSM.Heal(rst);
  ;
  while (digitalRead(PIR) == DETECTED) {
    if ((millis() / 10) - timer >= DEADLINE) { //1000) {
      //      digitalWrite(BUZZ, BUZZON);
      SMSreply();
      Serial.println(F("INCURSION DETECTED"));
      timer = millis() / 10;
    }
  }
  if (digitalRead(PIR) == nDETECTED) {
    timer = millis() / 10;
    //    digitalWrite(BUZZ, BUZZOFF);
  }
  //  digitalWrite(BUZZ, HIGH);
  //DETECT INCURSSION THEN SEND SMS
  //  Serial.println(analogRead(PIR));
}
