/*// NOT SAVED
   AT+GSMBUSY=1
   AT+CSCLK=0//2//1-> IF DTR CONNECTED
  //CAN BE SAVED BY AT&W0

  //////////////////////
  AT+CMGF=1
  AT+CMGS="7903461334"

  AT+HTTPSCONT Save HTTP Application Context
*/

//AT+HTTPTERM
//AT+HTTPINIT
//AT+HTTPPARA="URL","http://demo5089036.mockable.io/G_TEST"
//AT+HTTPPARA="CID","1"
//AT+HTTPPARA="CONTENT","application/json"
//AT+HTTPDATA=0,10000
//AT+HTTPACTION=1

/*
   POST:
  SIZE- 850 OK
  AT+HTTPPARA="URL","http://103.233.79.197:8000/ADSmart.asmx/Demo1
  AT+HTTPDATA=LEN,TIME
  >>DOWNLOAD>>DATA
  AT+HTTPDATA=335,1000
  AT+HTTPPARA="CONTENT","application/x-www-form-urlencoded" //VVI

  AT+HTTPSTATUS?
*/
//http://demo5089036.mockable.io/G_TEST

//POST /G_TEST HTTP/1.1
//Host: demo5089036.mockable.io
//Accept: */*
//User-Agent: Mozilla/5.0 (compatible; Rigor/1.0.0; http://rigor.com)
//Content-Length: 6
//Content-Type: application/x-www-form-urlencoded

#define SHOW_SERIAL 0

//CONFIG
//#define MAX_SLAVE     4//MAXIMUM SLAVE
//#define SAFE_RAM      500//MINIMUM SAFE RAM FOR ANOTHER TASK

//#define LEN_HEAD      5//ST. ADD-2, SLAVE.ID-1, MOD.FUN-1, 2*nBLOCK-1//IN BYTES

#define LEN_DELE      26//23//len_deleminator = ID=<2*4-Bytes>&DT=<2*4-Bytes>&G=<data-(dlen*2)> //1-CHAR 1-BYTES 4-BIT DATA//ID=00190014&DT=00000000&D=0EFF01030400000000/0F0102030400000000
#define RATIO_100     100//ms ratio
#define UPLOAD_MULT   ((1000/RATIO_100) *5)
#define UPLOAD_OFFSET 800/RATIO_100//ms offset//0.5sec OFFSET

/////////////// EEPROM ///////////////
// CONSTANTS
#define AT_         1
#define len_AT      2
#define HTTP        AT_+len_AT+1
#define len_HTTP    4
#define TERM        HTTP+len_HTTP+1
#define len_TERM    4
#define INIT        TERM+len_TERM+1
#define len_INIT    4

#define PARA_URL     5
#define PARA_CONTENT 6

///////////////////////

//#define DTR A1//7
#define ethRESET  3
//GSM
//#define GSM Serial//sim800l_test//Serial2
#define gsmbaud 19200
#define tout1 100
#define toutHTTPDATA 5000
#define toutSTATUS   20000//8000
#define toutHTTP     6000
#define DEBUG       0b00110001 //on error quick get error
//#define error       0b00110000
#define nZF1Eo1     0b10010001
#define ZF1Eo1      0b00010001
#define nZF1E0      0b10010000
#define ZF3oEo1     0b00111001
#define ZF3Eo1      0b00110001
#define ZF0Eo1      0b00000001
#define ZF3oE0      0b00111000
#define ZF3E0       0b00110000
#define ZF0E0       0b00000000
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

#define DGSM GSM
#define RX 4//16
#define TX 5//17

#define RST 6//18

volatile uint16_t count = 0;

uint32_t timer = 0;

uint8_t *data;//dynamic data pointer

//#include "Arduino.h"
//#include<TimerOne.h>
#include<SoftwareSerial.h>
#include<BhanuSTM32GSM.h>
//#include<BhanuEEPROM.h>

SoftwareSerial sim800l(RX, TX);//(RX,TX)

BhanuGSM GSM(sim800l);//type-Auto-0 OR GSM-1 OR ESP-2 OR ETHERNET-3 OR LORA-4 etc

//void ISR_T1() {
//
//}

bool HTTPInit() {
  //  AT+HTTPTERM
  //  GSM.CheckCmd(ReadeepString(AT_) + ReadeepString(HTTP) + ReadeepString(TERM), ZF3E0, tout1);
  //  GSM.CheckCmd(&ReadeepString(AT_), ZF3E0, tout1);
  GSM.CheckCmd(F("AT+HTTPTERM"), ZF3E0, tout1);

  //  AT+HTTPINIT
  //  return GSM.CheckCmd(ReadeepString(AT_) + ReadeepString(HTTP) + ReadeepString(INIT), ZF3E0, tout1);
  //  return GSM.CheckCmd(&ReadeepString(AT_), ZF3E0, tout1);
  return GSM.CheckCmd(F("AT+HTTPINIT"), ZF3E0, tout1);
}

uint8_t HTTPsetup() {
  //https://bhanu.free.beeceptor.com/G_POST
  //temp2239@gmail.com  - 1234567890
  GSM.CheckCmd(F("AT+HTTPPARA=\"URL\",\"http://gprstest.free.beeceptor.com/G_POST\""), ZF3E0, tout1);

  //  AT+HTTPPARA="URL","http://103.233.79.197:8000/ADSmart.asmx/Demo1
  //  GSM.CheckCmd(F("AT+HTTPPARA=\"URL\",\"http://demo5089036.mockable.io/G_TEST\""), ZF3E0, tout1);
  //  GSM.CheckCmd(F("AT+HTTPPARA=\"URL\",\"http://bhanu.free.beeceptor.com/G_POST\""), ZF3E0, tout1);
  //  GSM.CheckCmd(&ReadeepString(PARA_URL), ZF3E0, tout1);//SAVES

  //    AT+HTTPPARA="CONTENT","application / x - www - form - urlencoded" //VVI
  GSM.CheckCmd(F("AT + HTTPPARA = \"CONTENT\",\"application/x-www-form-urlencoded\""), ZF3E0, tout1);
  //  GSM.CheckCmd(&ReadeepString(PARA_CONTENT), ZF3E0, tout1);//SAVES
  return 1;
}

uint8_t Req_UPLOAD(uint16_t l) {
  String st;
  st = F("AT+HTTPDATA=");
  st += (String)l;
  st += F(",10000");
  //  Serial.println(st);
  if (!GSM.CheckCmd(&st, F("DOW"), ZF3E0, tout1))
    return 0;
  return 1;
}

uint8_t HTTPstatus() {
  //  AT+HTTPSTATUS?
  // +HTTPSTATUS: POST,1<0-idle, 1-Recv.., 2-Sending>,0<finish>,241<remaining to be sent or recv>
  //  Serial.println(F("STATUS"));
  delay(20);//relaxation
  uint32_t t = millis() / 100;
  while (GSM.Getuint8(F("AT+HTTPSTATUS?"), F(","), 1) != 0) {
    if (((millis() / 100) - t) > (toutSTATUS / 100))
      return 0;
  }
  return 1;
}

uint32_t Response() {
  // SAFELY Check for +HTTPACTION: 1<POST>,200<OK>,3<LEN>
  // Serial.println(GSM.GetCmd(F(""), F("+HTTP"), ZF0E0, toutHTTP));

  //  Serial.println(GSM.GetCmd(F(""), F("\r"), ZF0E0, tout1));
  String st;
  st = GSM.GetCmd(F(""), F("ON:"), ZF0Eo1, toutHTTP);//+HTTPACTION:
  if (!CheckString(&st, F("+HT"), 3)) { // && CheckString(&st, F(":"), 1)) {
    goto err;
  }
  st = GSM.GetCmd(F(""), F("\n"), ZF0Eo1, toutHTTP);//f2e01
  //  Serial.println(st);
  if (!CheckString(&st, F(" "), 1)) {
    goto err;
  }

  //extract useful data from ": 1,200,3"
  // CHECK method(OPTIONAL)
  //  Serial.println(GetInt(&st, F(" ")));//1<TYPE 0-GET, 1-POST>

  //  Serial.println(GetInt(&st, F(",")));//<HTTP STATUS REPLY>
  //  Serial.println(GetInt(&st, F(","), 2)); //<len>


  return ((GetInt(&st, F(","), 2) << 16) | GetInt(&st, F(",")));
err:
  return 0;
}

uint8_t Reply() {
  uint32_t r = Response();//0-15-> reply status, 16-31->len
  if (SHOW_SERIAL)
    Serial.println(r, HEX);
  // Return reply status ONLY IF LENTH IS <300
  // check reply status (MASK 0x00000FFC)
  //  r&0x00000FFC -> check series - 1xx, 2xx, 3xx, 4xx, 5xx, 6xx
  if (((r & 0x00000FFC) == 200)) {
    // r&0x00000003
    // check suitable length
  }
  ///IF DNS ERROR(603) close GPRS
  return 1;
}

uint8_t U_TEST(uint16_t dlen) {
  //if not data return
  if (dlen == 0)
    goto fail;
beg:
  //  Serial.println(F("U..."));
  //  Serial.println(GSM.GetSignal(SigPct));
  //CHECK GPRS
  //CPNNECT INTERNET
  if (!GSM.Internet()) {//0b01000000
    if (SHOW_SERIAL)
      Serial.println(F("!g"));
    //CHECK SIM OR HEAL GSM
    goto retry;
  }
  //  POST:
  //  SIZE- 850 OK
  if (!HTTPInit())
    goto retry;

  HTTPsetup();

  //calculate byte length of data
  //LEN_DELE = DEV=<2*2-Bytes>&DT=<2*4-Bytes>&d=<data-(dlen*2)>
  //Total len of frame = (2*dlen) + (LEN_DELE)


  //    check for:-
  //    DOWNLOAD
  //  Serial.println(F("HTTPDATA"));
  if (!Req_UPLOAD(dlen + LEN_DELE)) { //DOWNLOAD
    goto retry;
  }

  //ID=
  DGSM.Print(F("ID="));
  //  DGSM.Print(h2s(&String(Read16Int(CLIENT_ID), HEX), 16));//2B
  //  DGSM.Print(h2s(&String(Read16Int(DEVICE_ID), HEX), 16));//2B
  DGSM.Print(h2s(String(1, HEX), 16));//2B
  DGSM.Print(h2s(String(1, HEX), 16));//2B

  //&DT=
  DGSM.Print(F("&DT="));
  DGSM.Print(h2s(String(0, HEX), 32));//unix time//4B

  //&G=
  DGSM.Print(F("&G="));

  //  AT+HTTPDATA=LEN,TIME
  //  >>DOWNLOAD>>DATA
  //  AT+HTTPDATA=335,1000

  //  if (!data[index])
  //    continue;

  while (dlen > 0) {
    DGSM.Print(F("#"));
    dlen--;
    //    Serial.print(F("#"));
  }

  //    check for:-
  //    OK
  if (!GSM.CheckCmd(F(""), ZF3E0, tout1)) {
    goto retry;
  }

  //  AT+HTTPACTION=1
  //  Serial.println(F("ACT=1"));
  if (!GSM.CheckCmd(F("AT+HTTPACTION=1"), ZF3E0, tout1)) {//POST
    goto retry;
  }
  //  AT+HTTPSTATUS?
  //  Serial.println(F("STATUS"));
  //  Serial.println(GSM.GetCmd(F("AT+HTTPSTATUS?"), ZF3E0, tout1));

  //check STATUS
  if (!HTTPstatus()) {
    goto retry;
  }

  //////////// VERY SENSITIVE PART /////////////
  /*
      Accepted:- 200, 202,
      Rejected:-
      ERROR:-
  */


  if (!Reply()) {
    goto retry;
  }

  //read data if Reply() is ok
  if (SHOW_SERIAL) {
    Serial.println(F("READ"));
    Serial.println(GSM.GetCmd(F("AT+HTTPREAD"), ZF3E0, tout1));
  }
  //ACCOTDING TO STATUS READ HTTP MSG

ok:
  if (SHOW_SERIAL)
    ram();
  return 1;

retry:
  goto beg;

fail:
  //RESTART REQUEST
  return 0;
}


uint8_t U_TEST_() {
  if (SHOW_SERIAL)
    Serial.println(GSM.GetSignal(SigPct));
  //CHECK GPRS
  //CPNNECT INTERNET
  if (!GSM.Internet()) {//0b01000000
    if (SHOW_SERIAL)
      Serial.println(F("!g"));
    goto out;
  }
  //  POST:
  //  SIZE- 850 OK
  if (!HTTPInit())
    goto out;

  HTTPsetup();

  if (SHOW_SERIAL)
    Serial.println(GSM.GetCmd(F("AT+HTTPPARA?"), ZF3E0, tout1));
  //  AT+HTTPDATA=LEN,TIME
  //  >>DOWNLOAD>>DATA
  //  AT+HTTPDATA=335,1000

  //  AT+HTTPSTATUS?

  return 1;
out:
  return 0;
}


void setup() {
  // put your setup code here, to run once:
  pinMode(ethRESET, OUTPUT);
  digitalWrite(ethRESET, LOW);
  //  pinMode(DTR, INPUT_PULLUP);
  //  Timer1.initialize(100000);//100ms
  //  Timer1.attachInterrupt(ISR1);
  if (SHOW_SERIAL)
    Serial.begin(115200);

  sim800l.begin(gsmbaud);
  //  sim800l.setTimeout(100);
  //  Serial.setTimeout(200);
  //  delay(2000);
  //  sim800l.println(F("AT"));
  //  Serial.println(sim800l.readString());
  GSM_Heal();
  //  GSM.ConnectNTP();
  //  SMSreply();
  timer = millis() / 10;
  //  delay(5000);
}

//7903461334-BHANU
//9560566599-SUMIT BHAIA
//7979980112-SATYAM
//8116175026-RAJNISH

//void SMSreply()
//{
//  Timer1.detachInterrupt();
//  GSM.CheckCmd(F("AT+CMGF=1"), F("OK"), ZF3Eo1, tout1);
//  if (GSM.CheckCmd(F("AT+CMGS=\"9560566599\""), F(">"), ZF3Eo1, tout1 + 100)) {
//    //    GSM.println(F("Warning:- SOME INCURSSION DETECTED INSIDE CHAMBER FOR LAST 10 SECOND\nThis is Auto Generated Message by BHANU INCURSION DETECTOR"));
//    GSM.println(F("Alert: A SECURITY BREACH HAS BEEN DETECTED BY DEVICE-1"));
//    GSM.TermSend(0x03);
//  }
//  Timer1.attachInterrupt(ISR_T1);
//}

void 
{
  if (GSM.available() > 0)
  {
    Serial.println(F("Reading.."));
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
  if (Serial.available() > 0) {
    String st;
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
      if (CheckString(&st, F("C_"), 2)) {
        U_TEST_();
        return;
      }
      else if (CheckString(&st, F("CON"), 3) || CheckString(&st, F("con"), 3)) {
        ram();
        U_TEST(1);
        return;
      }
      Serial.println(st);

      //      GSM.println(st);
      st = GSM.GetCmd(&st, F("OK"), ZF1Eo1, 5000);
      Serial.println(st);
    }
  }
}

void GSM_Heal() {
  GSM.Heal(RST);
  Serial.print(F("N/w "));
  if (!GSM.Test(NWtest)) {
    if (SHOW_SERIAL)
      Serial.println(F("!K"));
  }
  else {
    //    Serial.println(F("OK"));
    //    GSM.SetAPN(F("bsnlnet"));
    //    GSM.SetAPN();
    //    Serial.print(F("GPRS "));
    //    if (!GSM.Test(GPRStest))
    //      Serial.println(F("FAIL"));
    //    else
    //      Serial.println(F("OK"));
  }
  if (SHOW_SERIAL) {
    Serial.print(GSM.GetSignal(SigPct));
    Serial.println(F("%"));
    ram();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  GSM.Heal(RST);
  uint32_t t = millis() / RATIO_100;
  //for (int16_t i = 2; i > 0; --i) {
  //    GSM.Print(F("#"));
  //    Serial.print(F("#"));
  //  }

  //MENU FOR SERIAL CONFIG

  //  GSM_Heal();
  //  U_TEST(1);
  //  while (!U_TEST(1));
  while (1) {
    if (SHOW_SERIAL)
      ;
    if ((millis() / RATIO_100) - t > 1000 / RATIO_100) {
      U_TEST(100);
      t = millis() / RATIO_100;
    }
  }
  //  Serial.print(GSM.GetSignal(SigPct));
  //  Serial.println(F("%"));
}
