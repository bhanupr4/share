//SIM900A AT COMMAND
//AT+CPOWD=1 SAFE POWER OFF
//AT+CPOWD=0 URGENT POWER OFF
//max http para = 450Bytes for SIM900A GET
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

void boot()
{ pinMode(42, OUTPUT);//R M
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
}
uint16_t baud[10] = {3, 12, 24, 48, 96, 192, 384, 576, 7488, 1152}; //* 100
String st;
//baud scanner
void setup() {
  // put your setup code here, to run once:
  boot();
  st.reserve(400);
  Serial.begin(19200);
  Serial2.begin(9600);
  Serial2.setTimeout(2000);
  Serial.println(F("AT+CCLK?"));
  Serial2.println(F("AT+SAPBR=4,1"));
  //  Serial.println(Serial2.readString());
  read2st();
  Serial.println(st);
}

void read2st()
{ st = "";
  char buff;
  uint32_t timer = millis();
  //use timer of 500 to wait and timeout
  while ((Serial2.available() <= 0) && ((millis() - timer) < 500));
readSerial:
  if (Serial2.available() > 0)
  { st += Serial2.readStringUntil('O');
    delay(2);//wait for Serial2
    buff = Serial2.read();
    if (buff == 'K')
    {
      //      Serial.println(F("OK"));
      st += 'O';
      st += buff;
    }
    else if (buff == 'R')
    {
      //      Serial.println(F("ERROR"));
      st += 'O';
      st += buff;
    }
    else
    { st += 'O';
      st += buff;
      delay(2);//wait for Serial2
      goto readSerial;
    }
  }
  //IF ANOTHER DATA COMED ELSE THAN OK or ERROR
  if (st == NULL && Serial2.available() > 0)
  { Serial.println(F("St read"));
    st = Serial2.readString();
  }
  S2flush();
  return;
}

void S2flush()//serial flush best for 9600
{ uint8_t b;
  delay(2);
  while (Serial2.available() > 0)
  { b = Serial2.read(); //flush serial
    delay(2);//relax time
  }
  return;
}

void loop() {
  // put your main code here, to run repeatedly:
  //read untill ok or error
  if (Serial2.available() > 0)
  { Serial.println(F("Reading.."));
    //    char b = 'O';
    read2st();
    Serial.println(st);
  }
  if (Serial.available() > 0)
  { String st;
    st = Serial.readString();
    Serial.println(st);
    Serial2.println(st);
  }
}

//
//void loop() {
//  // put your main code here, to run repeatedly:
//  //read untill ok or error
//  if (Serial2.available() > 0)
//  { Serial.println(F("Reading.."));
//    String s = "";
//    //    char b = 'O';
//readSerial:
//    if (Serial2.available() > 0)
//    { s += Serial2.readStringUntil('O');
//      delay(2);//wait for Serial2
//      char buff = Serial2.read();
//      if (buff == 'K')
//      {
//        //      Serial.println(F("OK"));
//        s += 'O';
//        s += buff;
//      }
//      else if (buff == 'R')
//      {
//        //      Serial.println(F("ERROR"));
//        s += 'O';
//        s += buff;
//      }
//      else
//      { s += 'O';
//        s += buff;
//        goto readSerial;
//      }
//    }
//    //IF ANOTHER DATA COMED ELSE THAN OK
//    if (s == NULL && Serial2.available() > 0)
//      s = Serial2.readString();
//    S2flush();
//    Serial.println(s);
//  }
//  if (Serial.available() > 0)
//  { String st;
//    st = Serial.readString();
//    Serial.println(st);
//    Serial2.println(st);
//  }
//}
