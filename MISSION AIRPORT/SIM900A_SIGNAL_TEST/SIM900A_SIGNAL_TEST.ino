//SIM900A AT COMMAND
//AT+CPOWD=1 SAFE POWER OFF
//AT+CPOWD=0 URGENT POWER OFF
//max http para = 450Bytes for SIM900A
//AT+HTTPPARA="URL","http://103.233.79.197:8000/ADSmart.asmx/Demo1?DT=0&DEV=SGA0001&DATA=____59a259a240c6_0c56b3f40c559a240c6_0c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832_42c800003f80000043635f2b4363601543636ed443c4e8ea43c4efb243c4ef4c3cee0e653cedcfaf3cee585c3f7561b13f7561b13f761a4a4247eb8740c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832_42c800003f80000043635f2b4363601543636ed443c4e8ea43c4efb243c4ef4c3cee0e653cedcfaf3cee585c3f7561b13f7561b13f761a4a4247eb8740c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832"
/*
   Time command
   AT+CLTS - n/w time sync
 * *PSUTTZ: REFRESH TIME AND TIME ZONE

   Command to read the GSM RTC clock :

  WAIT FOR +CTZV: +22
  IF(TIME.UNIX <1557290878)
  SYNC TIME AND RESTSRT MODULE
  AT+CCLK?

  Command to enable GSM RTC network time sync :
  AT+CLTS=1

  Command to save the command settings to memory so that the time is synchronized on every boot up automatically.
  AT&W
  RESART MODULE
*/
#include<TimerOne.h>
#define gsmbaud 9600

String st;
int s;
volatile bool state;
int index = -1;

void UPLOAD_BLINK_WAIT_off()
{ if (state == true)
    digitalWrite(43, 1);
  state = false;
  Timer1.initialize(50000);
  Timer1.attachInterrupt(UPLOAD_BLINK_WAIT_on);
}
void UPLOAD_BLINK_WAIT_on()
{ if (state == false)
    digitalWrite(43, 0);
  state = true;
  Timer1.initialize(400000);
  Timer1.attachInterrupt(UPLOAD_BLINK_WAIT_off);
}

void FAIL_BLINK()
{ state = !state;
  digitalWrite(42, state);
}

//boot
void boot()
{
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);
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
  digitalWrite(43, 1);
  delay(15);
  digitalWrite(44, 1);
  delay(15);
  digitalWrite(45, 1);
  delay(15);
  digitalWrite(46, 1);
  delay(400);
  digitalWrite(42, 0);
  digitalWrite(47, 0);
  digitalWrite(43, 0);
  digitalWrite(44, 0);
  digitalWrite(45, 0);
  digitalWrite(46, 0);
}

void signaal_led(uint8_t a, uint8_t b, uint8_t c)
{ //turn off all led
  digitalWrite(44, c); //rs485  1
  digitalWrite(45, b);  //2
  digitalWrite(46, a);  //3
}

//void boot()
//{ pinMode(42, OUTPUT);//R M
//  digitalWrite(42, 1);
//  delay(150);
//
//  pinMode(43, OUTPUT);//G U
//  digitalWrite(43, HIGH);
//  delay(50);
//  digitalWrite(42, 0);
//  delay(150);
//
//  pinMode(47, OUTPUT);//Y S
//  digitalWrite(47, 1);
//  delay(50);
//  digitalWrite(43, LOW);
//  delay(150);
//  digitalWrite(47, 0);
//  delay(150);
//  digitalWrite(42, 1);
//  delay(15);
//  digitalWrite(47, 1);
//  delay(15);
//  digitalWrite(43, HIGH);
//  delay(400);
//  digitalWrite(42, 0);
//  digitalWrite(47, 0);
//  digitalWrite(43, 0);
//}
//uint16_t baud[10] = {3, 12, 24, 48, 96, 192, 384, 576, 7488, 1152}; //* 100
//baud scanner
void setup() {
  // put your setup code here, to run once:
  boot();
  Serial.begin(19200);
  Serial2.begin(9600);
  Serial2.println(F("AT"));
  Serial.println(Serial2.readString());
}

bool test_esp()//PASS
{ Serial2.begin(gsmbaud);
  Serial2.println(F("AT"));
  wait_ok();
  //  if (!checkstring(st, F("OK"), 2))
  //  { Serial.println(F("ESP FAIL"));
  //    return false;
  //  }
  if (checkstring(&st, F("OK"), 2))
  { Serial.println(F("GSM OK"));
    if (s == 0)
      Timer1.detachInterrupt();
    digitalWrite(42, 1);
    return true;
  }
  else
  { Serial.println(F("GSM FAIL\nGive Power!"));
    Timer1.initialize(500000);
    Timer1.attachInterrupt(FAIL_BLINK);
    signaal_led(0, 0, 0);
    return false;
  }
}

void wdrst()
{
  //  wdt_reset();
  //  accumulate();
}

bool wait_ok()//PASS
{ //  Serial.println(F("Wait OK"));
  uint32_t t = millis();
  wdrst();
  st = Serial2.readString();
  //Serial.println(st);
  while (!checkstring(&st, F("OK"), 2))
  { wdrst();
    //    Serial.println(st);
    if (checkstring(&st, F("ERROR"), 5))
      return true;
    if ((millis() - t) > 3000)
    { return false;
    }
    Serial.print(F("."));
    st = Serial2.readString();
  }
  return true;
}

bool checkstring(String *source, String stemp, uint8_t len)
{ /*Serial.print(F("source="));Serial.print(source);Serial.print(F("\t to find="));
  */
  //  Serial.println(*source);
  //*c add 1 to trace string **c
  char **c, **d;
  uint16_t ad = 0;
  //  Serial.print(stemp);
  //  Serial.println();
  //  Serial.println(F("check st"));
  //  ad = source;
  //  c = ad;
  c = (uint16_t)source;
  d = c;
  //  Serial.println(ad);
  //  Serial.println((char)**c);
  //  Serial.println((char)**d);
  wdrst();
  if (*source == NULL)
  {
    //    Serial.println(F("Invalid parameter top"));
    return false;
  }
  if (*source != NULL)
  {
    for (uint16_t i = 0; **c != NULL; i++) ///////////////
    {
      wdrst();
      //      print_page();
      uint8_t j = 0;
      //Serial.println(F("(1)"));
      d = c;
      while (stemp[j] != NULL)
      { //Serial.println(F"last one founded"));
        //        accumulate();
        wdrst();
        //        print_page();
        //        Serial.print(F("**d="));
        //        Serial.println(**d);
        if (**d == stemp[j]) //change+
        { /*Serial.print(F("top i="));Serial.println(i);Serial.print(F("top sub found at j="));Serial.println(j);*/
          //          index = i + j;//GIVE FALSE INDEX SOME TIME
          //          c = ad;
          *d += 1;
          j++;
        }
        if (**d != stemp[j])
        {
          //          Serial.print(F("not usub found "));
          break;
        }
      }
      //Serial.println(F("while ended"));
      if (stemp[j] == NULL)
      { //index = (index + tlen) - 1;
        /*Serial.println(F("Top Whole string matched and found at index "));Serial.print(source[index]);Serial.println();*/
        //        c = ad;
        index = ad + len - 1;
        ad += j;
        *c -= ad;
        //        Serial.println((char)**c);
        //        Serial.println(F("String matched"));
        wdrst();
        return true;
      }
      //      c = ad;
      *c += 1;//increase to point next
      ad++;
      ad += j; //to solve missing string problem
      //      Serial.println();
      //      Serial.println((char)**c);
      //      Serial.println(*source);
    }
  }
  //  Serial.println(F("Whole string not found"));
  *c -= ad;
  //  Serial.println((char)**c);
  index = -1;
  wdrst();
  return false;
}


//faster GLOBAL DATA CASTING
uint8_t s8i(String *s)
{
  uint8_t d = 0;
  uint8_t i = 0;
  char **c;
  c = (uint16_t)s;
  while (**c != '\0')
  {
    if (i != 0)
      d = d * 10;
    d += (**c - 48);
    i++;
    *c += 1;
  }
  *c -= i;
  return d;
}
void signaal()
{
  if (test_esp())
  { //+CSQ: 25,0
    Serial2.println(F("AT+CSQ"));
    String temp = Serial2.readString();
    String temp1;
    //100% 31 0-31
    if (checkstring(&temp, F("SQ: "), 4))
    { index += 1;
      while (temp[index] != ',')
      { temp1 = temp1 + temp[index];
        index++;
      }
    }
    Serial.print(F("SIGNAL: "));
    uint8_t sig = 0;
    sig = s8i(&temp1);
    //show in led
    //turn off all led
    //    signaal_led(0, 0, 0);
    if ((sig != 0.0) && (sig <= 10)) {
      signaal_led(1, 0, 0);
    }
    else if ((sig > 10) && (sig <= 20)) {
      signaal_led(1, 1, 0);
    }
    else if (sig > 20) {
      signaal_led(1, 1, 1);
    }
    Serial.print((sig / 31.0) * 100);
    Serial.println(F("%"));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //  if (Serial2.available())
  //  {
  //  }
  //  if (Serial.available())
  //  { String st;
  //    st = Serial.readString();
  //    Serial.println(st);
  //    Serial2.println(st);
  //  }
  signaal();
  delay(2000);
}
