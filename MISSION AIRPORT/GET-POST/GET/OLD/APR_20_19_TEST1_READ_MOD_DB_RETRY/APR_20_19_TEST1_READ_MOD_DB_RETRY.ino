//MAIN CODE TEST1
//GPRS 490Bytes maximum
//AT+HTTPPARA="URL","http://103.233.79.197:8000/ADSmart.asmx/Demo1?DT=0&DEV=SGA0001&DATA=____59a259a240c6_0c56b3f40c559a240c6_0c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832_42c800003f80000043635f2b4363601543636ed443c4e8ea43c4efb243c4ef4c3cee0e653cedcfaf3cee585c3f7561b13f7561b13f761a4a4247eb8740c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832_42c800003f80000043635f2b4363601543636ed443c4e8ea43c4efb243c4ef4c3cee0e653cedcfaf3cee585c3f7561b13f7561b13f761a4a4247eb8740c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832"
/*
    IP 4 B
  IP4 1B+1 uint8_t  1                   539
  IP3 1B+1 uint8_t  2                   540
  IP2 1B+1 uint8_t  3                   541
  IP1 1B+1 uint8_t  4                   542

  PORT 2B+1 uint16_t 5 - 6 + 1          543

  UPLOAD 1B+1 unt8_t 8                  546

  CLIENT ID 2B+1  uint16_t  9 - 10 + 1  547

  DEVICE ID 2B+1  uint16_t  12 - 13 + 1 550

  CAPTURE ID 1B+1  uint8_t 15           554

  //MODBUS
  START ADD 2B+1 uint16_t 16 - 17 + 1 //not use
  N-POLL    1B+1 uint8_t  19          //not use
  SLAVE ID  1B+1 uint8_t  20          //not use
  MOD FUN   1B+1 uint8_t  21          //not use
  PARITY    1B+1 uint8_t  22            555
  BAUD      1B+1 uint8_t  23            556
  STOP-BIT  1B+1 uint8_t  24//not use
  RETRY                   24            557

  start ID

  //max data length 400
  data len=no._reg*4+4+2*3
*/
#include<avr/wdt.h>
#include <ModbusMaster.h>//Modbus
#include<EEPROM.h>
#include<TimerOne.h>

//GSM
#define gsmbaud 9600
//MODBUS
#define MAX485_DE   49//3
//#define timeout 1000
//#define polling 500//200 // the scan rate
#define timeout 10
#define polling 1000 // the scan rate


// instantiate ModbusMaster object
ModbusMaster node;
//for Modbus list
struct modbus {
  byte ID;//ID FOR MOD PARAMETER
  uint8_t slave;
  uint8_t fun;
  uint16_t address;
  uint8_t registers;
} mod;

struct mod
{
  // can be read from eeprom
  //  uint16_t address;
  //  uint8_t slave_id;
  //  uint8_t fun;
  int8_t result = -1; //for status of read
  uint16_t *d;//dynamic
} d_mod[50];

void safe_del(uint16_t **p)
{ delete *p;//work for all
  *p = NULL;
}

union
{ uint32_t d;
  float f;
} u;

//global
String st;
uint8_t n_fail = 0;
bool internet = false;
//for ISR
volatile bool state = false;
//EEPROM READ DATA
String ip_var, port_var;
uint8_t upload = 6, capture_id_var = 1, s = 0; //uload time
//RS485
uint16_t client_id_var, device_id_var;
uint8_t mod_parity_var, mod_retry_var;
uint16_t mod_baud_var = 96; //baud*100 38400 -- 384>225

//for internal eeprom
uint16_t add_ID = 600;
//uint16_t start_add_ID = 600;
uint8_t nBlock = 50;//SUPPORT 100 ENTRY
uint16_t last_ID = ((nBlock*sizeof(mod) + 600) - sizeof(mod));

//timer
uint32_t tu = 0;
uint32_t modbus_timer = 0;
int index = -1;

//SIGNAL
uint8_t sig = 0;

//ISR
void UPLOAD_BLINK()
{ state = !state;
  digitalWrite(43, state);
}

void FAIL_BLINK()
{ state = !state;
  digitalWrite(42, state);
}

void setWDT(byte sWDT)
{ WDTCSR |= 0b00011000;
  WDTCSR = sWDT | WDTO_2S;//set WDT BASED USER SETTING AND FOR 2 SECONDS INTERVAL
  wdt_reset();//call wdtreset() to tell wdt controller is working
}

void wdrst()
{ wdt_reset();
  //  accumulate();
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

//led
void fail(uint8_t pin)
{
  digitalWrite(pin, 1);
  delay(150);
  digitalWrite(pin, 0);
  delay(150);
  digitalWrite(pin, 1);
  delay(150);
  digitalWrite(pin, 0);
  if (pin == 43)
    state = false;
}

void get_ip()
{
  ip_var = (String)EEPROM.read(539) + F(".") + (String)EEPROM.read(540) + F(".") + (String)EEPROM.read(541) + F(".") + (String)EEPROM.read(542);
  Serial.print(ip_var);
  Serial.println(F("<--IPv4"));
}

void get_port()
{
  port_var = read16int(543);
  Serial.print(port_var);
  Serial.println(F("<--PORT"));
}

void get_upload()
{ upload = EEPROM.read(546);
  if (upload <= 1 || upload > 60)
    upload = 4;// * 5-sec//20sec
  Serial.print(upload * 5);
  Serial.println(F("<--U(s)"));
}

void get_capture_id()
{ capture_id_var = EEPROM.read(554);
  if (capture_id_var == 0 || capture_id_var >= 3)
    capture_id_var = 1;// default
  Serial.print(capture_id_var);
  Serial.println(F("<--CID"));
}

//RS485
void get_client_id()
{ client_id_var = read16int(547);
  Serial.print(client_id_var);
  Serial.println(F("<--CID"));
}

void get_device_id()
{ device_id_var = read16int(550);
  Serial.print(device_id_var);
  Serial.println(F("<--DID"));
}

void get_mod_parity()
{ mod_parity_var = EEPROM.read(555);
  if (mod_parity_var > 2)
    mod_parity_var = 0;
  Serial.print(mod_parity_var);
  Serial.print(F("<--P "));
  if (mod_parity_var == 0)
    Serial.println(F("N"));
  else if (mod_parity_var == 1)
    Serial.println(F("O"));
  else if (mod_parity_var == 2)
    Serial.println(F("E"));
}

void get_mod_baud()
{ uint8_t temp = EEPROM.read(556);
  if (temp == 1)
    mod_baud_var = 48;
  else if (temp == 2 || temp > 4)
    mod_baud_var = 96;
  else if (temp == 3)
    mod_baud_var = 192;
  else if (temp == 4)
    mod_baud_var = 384;
  Serial.print(mod_baud_var * 100);
  Serial.println(F("<--MOD baud"));
}

void get_mod_retry()
{
  mod_retry_var = EEPROM.read(557);
  if (mod_retry_var > 20 || mod_retry_var == 0)
    mod_retry_var = 3;
  Serial.print(mod_retry_var);
  Serial.println(F("<--MOD RETRY"));
}
//////////////////////////////////////////////////////////////////
//FASTER AND TAKE SAME RAM AS POINTER advanced
void mod_write_para(uint8_t id, uint8_t slav, uint8_t func, uint16_t addr, uint8_t nreg) //Advanced
{ wdrst();
  //  Serial.println(freeRam());
  //  0xFF is terminator
  if (id == 0)
    return 0;
  modbus save;
  save.ID = id;
  save.slave = slav;
  save.fun = func;
  save.address = addr;
  save.registers = nreg;
  //  Search for address of id in eeprom
  uint8_t eeprom;
  //  check for 0xFF before the id written
  for (eeprom = 1; eeprom < save.ID; eeprom++)
  { wdrst();
    add_ID = 600 + (eeprom - 1) * sizeof(mod);
    if (EEPROM.read(add_ID) == 0xFF)
      EEPROM.update(add_ID, 0);
  }
  add_ID = 600 + ((save.ID - 1) * sizeof(mod));
  { eeprom = EEPROM.read(add_ID);
    if (eeprom == 0xFF)//not found
      goto write;
    else if (eeprom == save.ID)//found
      goto overwrite;
    else if (eeprom > save.ID)//found
      goto recover;
  }
  //  if found then overwrite
overwrite:
  //  Serial.print(id);
  //  Serial.print(F(" is over Written at "));
  //  Serial.println(add_ID);
  goto exitt;
recover:
  add_ID -= sizeof(save);
  Serial.print(id);
  Serial.print(F(" is Recovered at "));
  Serial.println(add_ID);
  goto exitt;
  //  else write at empty space
write:
  //NEW DATA WRITE NEED TO CHECK INDEXING THEN REINDEX
  //  Serial.print(id);
  //  Serial.print(F(" is written at "));
  //  Serial.println(add_ID);
exitt:
  //  Serial.println(freeRam());
  EEPROM.put(add_ID, save);
  Serial.println(freeRam());
  wdrst();
}
void frame()
{ wdrst();
  Serial.print(F("|"));
  for (int i = 1; i <= 5; i++)
    Serial.print(F("------\t|"));
  Serial.println();
  Serial.println(F("|  SNo\t|  ID\t| FunC\t| Addr\t| POLLS\t|"));
  Serial.print(F("|"));
  for (int i = 1; i <= 5; i++)
    Serial.print(F("------\t|"));
  Serial.println();
}

uint8_t mod_read_para(uint8_t id)//advanced
{ wdrst();
  //  Serial.print(F("Reading data of ID-"));
  //  Serial.println(id);
  //same id should not avail twice
  uint8_t eeprom;
  //  for (add_ID = 600; add_ID <= last_ID; add_ID += sizeof(mod))
  add_ID = 600 + ((id - 1) * sizeof(mod));
  eeprom = EEPROM.read(add_ID);
  if (eeprom == 0x00) //not found
    goto not_found;
  else if (eeprom == 0xFF) //db end
    goto end_db;
  else if (eeprom == id)
  {
    //done:
    //Write data to variable 'o'
    //  Serial.println(add_ID);
    EEPROM.get(add_ID, mod);
    Serial.print(F("Showing Table: For ID-"));
    Serial.println(id);
    frame();
    Serial.print(F("|   "));
    Serial.print(mod.ID);
    Serial.print(F("\t"));
    Serial.print(F("|   "));
    Serial.print(mod.slave);
    Serial.print(F("\t"));
    Serial.print(F("|   "));
    Serial.print(mod.fun);
    Serial.print(F("\t"));
    Serial.print(F("| "));
    Serial.print(mod.address);
    Serial.print(F("\t"));
    Serial.print(F("|  "));
    Serial.print(mod.registers);
    Serial.print(F("\t"));
    Serial.println(F("|"));
    Serial.print(F("|"));
    wdrst();
    for (int i = 1; i <= 5; i++)
      Serial.print(F("------\t|"));
    Serial.println();
    Serial.println(F("Reading Done"));
    return 1;
  }
not_found:
  Serial.println(F("Already deleted"));
  return 0;
end_db:
  Serial.println(F("Db end"));
  return 2;
}


void mod_show_para()//advance
{ wdrst();
  modbus read;
  uint8_t no_data = 0;//let we have no data
  Serial.println(F("Reading Table:"));
  frame();
  for (add_ID = 600; add_ID <= last_ID; add_ID += sizeof(mod))
  { wdrst();
    EEPROM.get(add_ID, read);
    if (read.ID == 0xFF)
    { goto exitt;
    }
    else if (read.ID == 0)
    { no_data++;
      continue;
    }
    //    if (read.ID != i)
    //    { //re-indexing
    //      EEPROM.update(add_ID, i);
    //      EEPROM.get(add_ID, read);
    //    }
    Serial.print(F("|   "));
    Serial.print(read.ID);
    Serial.print(F("\t"));
    Serial.print(F("|   "));
    Serial.print(read.slave);
    Serial.print(F("\t"));
    Serial.print(F("|   "));
    Serial.print(read.fun);
    Serial.print(F("\t"));
    Serial.print(F("| "));
    Serial.print(read.address);
    Serial.print(F("\t"));
    Serial.print(F("|  "));
    Serial.print(read.registers);
    Serial.print(F("\t"));
    Serial.println(F("|"));
  }
exitt:
  Serial.print(F("|"));
  for (int i = 1; i <= 5; i++)
    Serial.print(F("------\t|"));
  Serial.println();
  wdrst();
  add_ID -= sizeof(mod);
  //  Serial.println(add_ID);
  //  Serial.println(last_ID);
  //  Serial.println(no_data);
  if (no_data == (((add_ID - 600) / sizeof(mod)) + 1))
  { Serial.println(F("No data"));
    while (no_data > 0)
    { Serial.print(F("Setting "));
      Serial.print(add_ID);
      Serial.println(F(" to 0xFF"));
      EEPROM.update(add_ID, 0xFF);
      add_ID -= sizeof(mod);
      no_data--;
      wdrst();
    }
  }
  wdrst();
}


void mod_del_para(uint8_t i, uint8_t f)//advanced
{ if (i == f)
    Serial.println(F("Single DELETE"));
  if (f < i)
  { Serial.println(F("Invalid Entery"));
    goto exitt;
  }//seek to address and set id to zero
  add_ID = 600 + ((i - 1) * sizeof(mod));
  for (i; i <= f; i++)
  { wdrst();
    if (EEPROM.read(add_ID) == 0xFF)
      goto exitt;
    EEPROM.update(add_ID, 0);
    //delete old data block
    safe_del(&d_mod[i - 1].d);
    //    Serial.print(i);
    //    Serial.print(F(" is deleted at "));
    //    Serial.println(add_ID);
    add_ID += sizeof(mod);
  }
exitt:
  Serial.println(F("Done deleting"));
}

//////////////////////////////////////////////////////////////


int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

//Global Function
void preTransmission()
{
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_DE, 0);
}

////////////////////// READ EEPROM ////////////
String readeepstring(uint16_t i)//i- starting address
{ //--//Serial.println(F("READING EEPROM String"));
  wdt_reset();
  String temp;
  for (uint16_t addr = i; ((char)EEPROM.read(addr) != '\0'); addr++)
  { temp += ((char)EEPROM.read(addr));
    wdt_reset();
    //Serial.println(temp);
  }
  wdt_reset();
  //--//Serial.println(temp);
  return temp;
}

uint16_t read16int(uint16_t address)
{ //read data from eep and return it
  wdt_reset();
  uint8_t temp = 0; uint16_t d = 0;
  address++;
  byte addlen = 2;
  //Serial.println();
  while (addlen > 0)
  { temp = EEPROM.read(address);
    //Serial.println(temp, HEX);
    d = (d << 8);
    d = d | temp;
    address--;
    addlen--;
    //    wdt_reset();
  }
  wdt_reset();
  //Serial.println();
  //Serial.println(d);
  return d;
}

//////////// WRITE EEPROM ////////////////
bool write16int(uint16_t address, uint16_t d)
{ //write data to eep then verify, if fail return false
  wdt_reset();
  uint8_t temp = 0;
  uint16_t addr = address;
  while (d != 0)//WRITING LSB TO MSB
  { wdt_reset();
    temp = d | temp;
    //Serial.println(temp, HEX);
    EEPROM.update(address, temp);
    delay(4);
    //++//Serial.println(EEPROM.read(address));
    if (EEPROM.read(address) != temp)
    { return false;
    }
    temp = 0;
    //Serial.println(d, BIN);
    d = (d >> 8) & 0x00FF;
    //Serial.println(d, BIN);
    address++;
    wdt_reset();
  }
  while ((address - addr) != 3)
  { EEPROM.update(address, 0);
    //++//Serial.println(EEPROM.read(address));
    address++;
    wdt_reset();
  }
  return true;
}

bool writeeepstring(uint16_t i, uint16_t l, String *temp) //i- initial address l- end address
{
  wdt_reset();
  if (((l - i) + 1) >= lengthof(temp))
  { uint16_t add;
    char **c = (uint16_t)temp;
    //    wdt_reset();
    //Serial.println(F("memory ok"));
    //    disable_intr();
    for (add = 0; **c != '\0'; add++)
    { EEPROM.update((add + i), **c);
      *c += 1;
      //Serial.print("add[");
      //Serial.print(add + i);
      //Serial.print("]=");
      //Serial.println((char)EEPROM.read(add + i));
      //      wdt_reset();
    }
    EEPROM.update((add + i), 0);
    *c -= add;
    //    Serial.println(*temp);
    //    enable_intr();
    return true;
  }
  else
  { //Serial.println(F("memory error"));
    return false;
  }
}

/////////////////////////////////////////
uint16_t lengthof(String *stemp)          //to find length of string
{ uint16_t len = 0;
  char **c;
  c = (uint16_t)stemp;
  for (uint16_t i = 0; **c != '\0'; i++)
  { len++;
    *c += 1;
    //    wdt_reset();
  }
  *c -= len;
  return len;
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

uint32_t s32i(String *s)
{ wdrst();
  //  timer = micros();
  uint32_t d = 0;
  uint8_t i = 0;
  char **c;
  c = (uint16_t)s;

  while (**c != '\0')
  { wdrst();
    if (i != 0)
      d = d * 10;
    d += (**c - 48);
    *c += 1;
    i++;
  }
  *c -= i;
  wdrst();
  //  Serial.println(freeRam());
  //  Serial.println(micros() - timer);
  return d;
}

String h2s(String *str, uint8_t len) //padding of zero in hex
{
  //  Serial.println(str);
  String s = "";
  uint8_t l = lengthof(str);//change-
  len = (len / 4);
  //  Serial.println(len);
  //  Serial.println(l);
  if (l < len)
  { //padding zero
    while (l != len)
    {
      s += F("0");
      l++;
    }
  }
  s += *str;
  //  Serial.println(s);
  return s;
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
    Serial.print(((float)sig / 31.0) * 100);
    Serial.println(F("%"));
  }
}

void heal_esp()//PASS
{ Serial.println(F("HEALING GSM"));
  if (!test_esp())
  { Serial.println(F("GSM FAILED HEALING"));
    restartesp(1);//with cmd
    setting();
  }
  //  digitalWrite(42, 1);
  //  Serial.println(F("GSM OK"));
}

void restartesp(bool cmd)//PASS
{ Serial.println(F("rst GSM"));
  wdrst();
  if (cmd == true)
    Serial2.println(F("AT+CPOWD=0"));
  wait_ok();
  //  st = Serial2.readString();
  if (!checkstring(&st, F("RDY"), 5))
  {
    //    waiit for  30 second
    wdrst();
    delay(2000);
    wdrst();
    Serial2.println(F("AT"));
    wdrst();
    wait_ok();
    wdrst();
    //--// Serial.println(st);
    if (!checkstring(&st, F("OK"), 2))
    { //Serial.println(F("Not Ready"));
      wdrst();
      return restartesp(0);
    }
  }
  wdrst();
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
    return false;
  }
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

void setting()//to set a sever //PASS
{ // put your setup code here, to run once:
  //DISABLE SLEEP MODE
  //DISSCONNECT CALL
  Serial2.begin(gsmbaud);
  Serial2.println(F("ATE0"));
  wait_ok();//wait for busy s.. string
  Serial2.println(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""));
  wait_ok();
  Serial2.println(F("AT+SAPBR=3,1,\"APN\",\"internet\""));
  wait_ok();//wait for busy s.. string
  if (!test_esp())
  { restartesp(1);
    wdrst();
    setting();
    wdrst();
  }
}

bool check_2g()
{
  Serial2.println(F("AT+SAPBR=2,1"));
  if (wait_ok())
  { if (checkstring(&st, F("PBR: "), 5))
    { //st[index] = P
      index += 3;
      Serial.println(st[index]);
      if (st[index] != '1')
      { //CONNECT GPRS AT+SAPBR=1,1
        Serial.println(st[index]);
        Serial.println(F("Connecting GPRS"));
        Serial2.println(F("AT+SAPBR=3,1,\"APN\",\"internet\""));
        if (!wait_ok()) //wait for busy s.. string
          return false;
        Serial2.println(F("AT+SAPBR=1,1"));//CONNECT BARRIER
        //Check for +SAPBR: 1,1
        if (!wait_ok())
          return false;
        return true;
      }
      return true;
    }
  }
}

bool upload_raju(String *d)//new capture then pass
{
  uint32_t upload_time = millis();
  //  Serial.println(F("UPLOAD_raju"));
  //connect and check server
  //blink upload led to tell connecting
  s = 1;
  Timer1.initialize(400000);//100ms
  Timer1.attachInterrupt(UPLOAD_BLINK);
  signaal();
  //Check gprs connection
  if (!check_2g())
  { n_fail++;
    internet = false;
    s = 4;//N
    goto debug;
  }
  //  Timer1.detachInterrupt();
  Timer1.initialize(100000);//100ms
  Timer1.attachInterrupt(UPLOAD_BLINK);
  s = 2;
  //  Serial.println(F("HTTPTERM"));
  Serial2.println(F("AT+HTTPTERM"));
  if (!wait_ok())
    goto debug;
  //  Serial.println(F("HTTPINIT"));
  Serial2.println(F("AT+HTTPINIT"));
  if (!wait_ok())
    goto debug;
  wdrst();
  st = F("AT+HTTPPARA=\"URL\",\"http://");
  st += ip_var; //IP1//CHANGE-
  //  st += F("103.233.79.197");
  st += F(":");
  st += port_var;//PORT//CHANGE-
  //  st += F("8000");
  st += F("/ADSmart.asmx/Demo1");//OLD
  //      st += F("/AdControl.aspx");//TEST
  //      st += server_id_var;
  st += F("?DT=0&DEV=SGA0001");//OLD
  //      st += F("?DT=0&DEV=V");//TEST
  //      //cid
  //      String temp = String(client_id_var.toInt(), HEX);
  //      st += h2s(&temp, 16); //length =4
  //      //Device id
  //      st += F("SGR");
  //  /CHANGE-
  //  String temp;
  //  temp.reserve(4);
  //  temp = String(device_id_var, HEX);
  //  st += h2s(&temp, 16); //length =4 CHANGE-/
  //      Serial.println(st);
  st += F("&DATA=");
  //6DIGIT DUMMY BY RAJU
  //      st += F("&DATA=ABCDEF");
  //I AM PUTTING 4-DIGIT CLIENT ID and 2-digit dummy
  //  Serial.println(*d);
  st += *d;
  st += F("\"");
  //  Serial.println(st);
  Serial2.println(st);
  if (!wait_ok())
    goto debug;
  //  Serial.println(F("Closing HTTP"));
  Serial2.println(F("AT+HTTPACTION=0"));
  if (wait_ok())
  {
    uint32_t t = millis() / 100;
    while (!checkstring(&st, F("TION:"), 5))//CHECK FOR +HTTPACTION
    {
      //      Serial.println(st);
      st = Serial2.readString();
      if (((millis() / 100) - t) > 100) //10SEC=5000/100=50
      { Serial.println(F("TIME OUT"));
        //        Serial2.println(F("AT+SAPBR=0,1"));//CHANGE+
        //        if (!wait_ok())
        //          return false;
        //        internet = false;
        s = 3;
        //        printtimer = millis();
        goto debug;
      }
      //      delay(100);
    }
    //+HTTPACTION:0,200,88
    s = 6;
    if (!checkstring(&st, F("200,"), 4))
      goto debug;
    Serial.println(F("LINK OK"));
    internet = true;
    Serial2.println(F("AT+HTTPREAD"));
    if (wait_ok())
    {
      uint32_t t = millis();
      while (!checkstring(&st, F("READ:"), 5))//CHECK FOR +HTTPREAD: LEN
      { //        print_page();
      }
      if (!checkstring(&st, F("ACK"), 3))//UPLOAD ACKNOWLEDGEMENT
        goto debug;
      Serial.println(F("UPLOAD DONE"));
      Timer1.detachInterrupt();
      digitalWrite(43, 1);
      Serial.println(millis() - upload_time);
      s = 7;
      return true;
      //      Serial.println(st[index]);
    }
debug:
    s = 0;
    Serial.println(F("UPLOAD FAIL"));
    Timer1.detachInterrupt();
    fail(43);
    Serial.println(st);
    return false;
  }
}

//RS485
void begin_rs485()
{ //RS485
  if (mod_parity_var == 0)
    Serial3.begin(mod_baud_var * 100, SERIAL_8N1); //TAKE USER INPUT
  else if (mod_parity_var == 1)
    Serial3.begin(mod_baud_var * 100, SERIAL_8O1); //TAKE USER INPUT
  else if (mod_parity_var == 2)
    Serial3.begin(mod_baud_var * 100, SERIAL_8E1); //TAKE USER INPUT
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void setup() {
  // put your setup code here, to run once:
  //watchdog ok
  wdt_disable();
  wdt_enable(WDTO_4S);
  boot();
  //  delay(200);
  Serial.begin(19200);
  Serial.println(freeRam());
  Serial.println(F("Fw: V-0.1"));
  port_var.reserve(4);
  ip_var.reserve(19);
  get_ip();
  get_port();
  get_device_id();
  get_client_id();
  get_upload();//CHANGE+
  get_capture_id();

  get_mod_parity();
  get_mod_baud();
  get_mod_retry();

  mod_show_para();
  Serial.print(F("Please Wait.."));
  Serial2.begin(gsmbaud);
  //RS485 SETUP
  pinMode(MAX485_DE, OUTPUT);
  begin_rs485();//rs485.begin(baud, NONE);
  setting();
  //  String data = F("____59a259a240c6_0c56b3f40c559a240c6_0c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832_42c800003f80000043635f2b4363601543636ed443c4e8ea43c4efb243c4ef4c3cee0e653cedcfaf3cee585c3f7561b13f7561b13f761a4a4247eb8740c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832_42c800003f80000043635f2b4363601543636ed443c4e8ea43c4efb243c4ef4c3cee0e653cedcfaf3cee585c3f7561b13f7561b13f761a4a4247eb8740c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832");
  //  upload_raju(&data);
  wdrst();
  //timer reset
  modbus_timer = millis() / 100;
  tu = millis();//timer to upload data
  Serial.println(F("SETUP DONE"));
  Serial.println(freeRam());
}


void read_data()
{ uint8_t j, result;//j- no. of attempts

  for (uint8_t i = 0; i < 50; i++)
  { //get and update j with mod_retry_var
    digitalWrite(44, 0);
    j = mod_retry_var - 1;
    digitalWrite(47, 0);//Y-S
    Serial.println(freeRam());
    if (mod_read_para(i + 1) != 1) //reading parameter from ram
    { if (mod_read_para(i + 1) == 0) {
        d_mod[i].result = 0; continue;
      }
      else if (mod_read_para(i + 1) == 2) { //ALL read
        d_mod[i].result = -1; return;
      }
    }
    else
    { //delete old
      safe_del(&d_mod[i].d);
      //ram para read success
      node.begin(mod.slave, Serial3);
retry:
      //function code 3
      wdrst();
      if (mod.fun == 3)
        result = node.readHoldingRegisters(mod.address, mod.registers);
      //function code 4
      else if (mod.fun == 4)
        result = node.readInputRegisters(mod.address, mod.registers);
      if (result != 0x00)
      { //IF RESULT!=0 ATTEMPT N-TIMES AS PER USER
        fail(44);
        if (result == 0x02)
          Serial.println(F("MODBUS: Illigal Data Add."));
        else if (result == 0x03)
          Serial.println(F("MODBUS: Illigal Data Value"));
        else if (result == 0x04)
          Serial.println(F("MODBUS: Slave Failure"));
        else if (result == 0xE1)
          Serial.println(F("MODBUS: Invalid Function"));
        else if (result == 0xE2)
          Serial.println(F("MODBUS: Timeout"));//RETRY
        else if (result == 0xE0)
          Serial.println(F("MODBUS: Invalid Slave id"));//RETRY
        else if (result == 0xE3)
          Serial.println(F("MODBUS: Invalid CRC"));
        if (j != 0) //since starting with zero
        { j--;
          goto retry;
        }
        else
        { //continue to for loop again
          d_mod[i].result = 0;
          continue;
        }
      }
      { //mod success
        digitalWrite(44, 1);//RED
        Serial.print(F("Reading Modbus.."));
        Serial.println(mod.ID);
        //calloc and reserve memory for string
        //delete old
        safe_del(&d_mod[i].d);
        d_mod[i].result = mod.registers;
        d_mod[i].d = (uint16_t*)calloc(mod.registers, sizeof(uint16_t));//uint16_t data

        //for 16-bit
        for (j = 0; j < mod.registers; j += 2)
        {
          //              mod_data[0] = node.getResponseBuffer(j);
          //              mod_data[1] = node.getResponseBuffer(j + 1);
          *(d_mod[i].d + j) = node.getResponseBuffer(j);
          *(d_mod[i].d + (j + 1)) = node.getResponseBuffer(j + 1);
          wdrst();
          { Serial.print(mod.address + (j + 1));
            Serial.print(F("="));
            u.d = *(d_mod[i].d + (j + 1));
            u.d = u.d << 16;
            u.d = u.d | *(d_mod[i].d + j);
            //            u.d = d32;
            //      Serial.println(data[j], HEX);
            //      Serial.print(u.d, HEX);
            Serial.print(u.d, HEX);
            Serial.print(F("="));
            Serial.println(u.f);
          }
        }
        //print result
        for (j = 0; j < d_mod[i].result; j++)
        {
          Serial.print(*(d_mod[i].d + j), HEX);
        }
        Serial.println();
        //            safe_del(&d_mod[i].d);
      }
    }
  }//END FOR
  return;
}

void loop()
{ wdrst();
  //  uint16_t mod_data[2];//Data buffer 124
done:
  if (digitalRead(47) == 0)
    digitalWrite(47, 1);
  Serial.println(F("WHILE 1"));
  while (1)
  { //WHILE1
    wdrst();
    if (((millis() / 100) - modbus_timer) > (upload * 5 * 10))//CHANGE-
    { //timer
      read_data();
      modbus_timer = millis() / 100;
      //      for (j = 0; j < 50; j++)
      //      { safe_del(&d_mod[j].d);
      //        Serial.println(d_mod[j].result);
      //      }
      //      safe_del(&d_mod[0].d);//work for all
      //      safe_del(&d_mod[1].d);//work for all
      //      safe_del(&d_mod[2].d);//work for all
      //      safe_del(&d_mod[3].d);//work for all
      //      safe_del(&d_mod[4].d);//work for all
      //      safe_del(&d_mod[5].d);//work for all
      //      safe_del(&d_mod[6].d);//work for all
      //      free(d_mod[0].d);
      //      free(d_mod[1].d);
      //      free(d_mod[4].d);
      //delete using realloc
      Serial.println(freeRam());
      goto done;
    }
  }
}
