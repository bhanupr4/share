//MAIN CODE TEST1
//GPRS 490Bytes maximum
//AT+HTTPPARA="URL","http://103.233.79.197:8000/ADSmart.asmx/Demo1?DT=0&DEV=SGA0001&DATA=____59a259a240c6_0c56b3f40c559a240c6_0c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832_42c800003f80000043635f2b4363601543636ed443c4e8ea43c4efb243c4ef4c3cee0e653cedcfaf3cee585c3f7561b13f7561b13f761a4a4247eb8740c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832_42c800003f80000043635f2b4363601543636ed443c4e8ea43c4efb243c4ef4c3cee0e653cedcfaf3cee585c3f7561b13f7561b13f761a4a4247eb8740c56b3f40c559a240c6cb7a431a74fe431ac20d431ba832"
//http://103.233.79.197:8000/ADSmart.asmx/Demo1?DT=1558031580&DEV=V0020&data=006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD
//http://103.233.79.197:8000/ADSmart.asmx/Demo1?DT=1558031580&DEV=V0020&data=0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D4500000000
//http://103.233.79.197:8000/ADSmart.asmx/Demo1?DT=1558031580&DEV=V0020&data=0F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

//http://103.233.79.197:8000/ADSmart.asmx/Demo1?DT=1558031580&DEV=V0020&data=006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF


/*
  POST:
  SIZE- 850 OK
  AT+HTTPPARA="URL","http://103.233.79.197:8000/ADSmart.asmx/Demo1
  AT+HTTPDATA=LEN,TIME
  >>DOWNLOAD>>DATA
  AT+HTTPDATA=335,1000
  AT+HTTPPARA="CONTENT","application/x-www-form-urlencoded" //VVI

   //link is case sensitive
   data string in upper case
  http://103.233.79.197:8000/ADSmart.asmx/Demo1?DT=0&DEV=V0020&data=0EFF02037C79004839790048390000000000000000000000000000000096A8439400000000BD004776BD00477600000000000000000000000000000000CF7D43949700477697004776000000000000000000000000000000004FBE43948F0047788F00477800000000000000000000000000000000A4FE4394C0934D3CC0764D3C
  POST SIZE: 1-102400 OR 1-318976 bytes
   WHEN TABLE ENTRY FINISH ARRANGE IT IN ASSENDING ORDER

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
#include<SoftwareSerial.h>//new

//GSM
#define GSM sim800l_test//Serial2
#define gsmbaud 19200//9600
//MODBUS
#define MAX485_DE   49//3
//#define timeout 1000
//#define polling 500//200 // the scan rate
#define timeout 10
#define polling 1000 // the scan rate

SoftwareSerial sim800l_test(13, 12);//(Rx,Tx)(8,7)
// instantiate ModbusMaster object
ModbusMaster node;
const uint8_t nBlock = 60;//SUPPORT 50 ENTRY
//for Modbus list
struct modbus {
  byte ID;//ID FOR MOD PARAMETER
  uint8_t slave;
  uint8_t fun;
  uint16_t address;
  uint8_t registers;
} mod;

struct upload_index {
  int8_t ix = -1;
} u_i[nBlock];

struct upload_data
{ String *data;
} u;

struct mod
{
  // can be read from eeprom
  //  uint16_t address;
  //  uint8_t slave_id;
  //  uint8_t fun;
  int8_t result = -1; //for status of read
  uint16_t *d;//dynamic
} d_mod[nBlock];

void safe_del(uint16_t **p)
{ delete *p;//work for all
  *p = NULL;
}

union
{ uint32_t d;
  float f;
} c;


//global
String st;//VVI
uint8_t n_fail = 0;//VVI USED EVERY TIME
bool internet = false;
//for ISR
volatile bool state = false;//VVI
//EEPROM READ DATA
String ip_var, port_var;//CAN READ FROM EEPROM DIRECTLY
uint8_t upload = 6, capture_id_var = 1, s = 0; //USED MANY TIME VVI
//RS485 -- CAN READ FROM EEPROM DIRECTLY
uint16_t client_id_var, device_id_var;
uint8_t mod_parity_var, mod_retry_var;
uint16_t mod_baud_var = 96; //baud*100 38400 -- 384>225

//for internal eeprom
uint16_t add_ID = 600;//VVI BUT CAN BE LOCALLY USED
//uint16_t start_add_ID = 600;
//uint8_t nBlock = 100;//SUPPORT 50 ENTRY
uint16_t last_ID = ((nBlock*sizeof(mod) + 600) - sizeof(mod));//MAY BE VVI

//timer
uint32_t tu = 0;
uint32_t modbus_timer = 0;
int16_t index = -1;//VVI

//SIGNAL
uint8_t sig = 0;//VVI

//ISR
void UPLOAD_BLINK()
{ state = !state;
  digitalWrite(43, state);
}

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

void signaal_led(uint8_t a, uint8_t b, uint8_t c)
{ //turn off all led
  digitalWrite(44, c); //rs485  1
  digitalWrite(45, b);  //2
  digitalWrite(46, a);  //3
}
//led
void fail(uint8_t pin)
{ for (uint8_t i = 1; i <= 4; i++)
  { digitalWrite(pin, (i % 2));
    if (i % 4 != 0)
      delay(150);
  }
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
    //delete old dynamic data block
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
int8_t sync() {//modbus data sync()
  if (d_mod[0].result == -1)
    return -1;
  int8_t j = -1;//j- for
  if (u_i[0].ix != -1)
  { j = 0;
    uint8_t mod_count = 0;
    //get no. of non zero data from d_mod
    for (uint8_t i = 0; i < nBlock; i++)
    { if (d_mod[i].result == -1)
        break;
      if (d_mod[i].result > 0)
        mod_count++;
    }//total no zero data count
    //    Serial.println(mod_count);
    while (j < nBlock)
    { if (u_i[j].ix == -1)
      { if (mod_count != j)
        { j = -1;
          u_i[0].ix = -1;
          goto recopy;
        }
        j--;
        //        Serial.println(F("Ac"));
        return j;
      }
      if (d_mod[u_i[j].ix].result == 0)
      { j = -1;
        u_i[0].ix = -1;
        //        /*0th to -1*/u_i[0].ix = -1;
        goto recopy;
      }
      j++;
    }
  }
recopy:
  //  Serial.println(F("Rc"));
  for (uint8_t i = 0; i < nBlock; i++)
  { if (d_mod[i].result == -1)
    { u_i[j + 1].ix = -1;
      //      Serial.println(j + 1);
      return j;
    }
    else if (d_mod[i].result != 0)
    { //check for max and swap if smallest found
      j++;
      if (d_mod[u_i[0].ix].result < d_mod[i].result)
      { u_i[j].ix = u_i[0].ix;
        u_i[0].ix = i;
      }
      else if (j != 0)
      { if (d_mod[u_i[j - 1].ix].result < d_mod[i].result) //in case of future -1 it false
        { u_i[j].ix = u_i[j - 1].ix;
          u_i[j - 1].ix = i;
        }
        else
          u_i[j].ix = i;
      }
    }
  }
  //if j is -1 no data copied
  return j;
}

int8_t sort()//working mberfine can check already sorted number
{ int8_t len = sync();
  if (len == -1)
    return -1;
  //check for already sorted
  //max value at index-0 as pivot
  //set start index which is not sorted
  int8_t temp;
  for (temp = 0; temp < len; temp++)
  { if (d_mod[u_i[temp].ix].result != d_mod[u_i[temp + 1].ix].result)
    { if ((d_mod[u_i[temp].ix].result - d_mod[u_i[temp + 1].ix].result) != 1)
      { temp++;
        goto sort;
      }
    }
  }
  //  Serial.print(F("Already sorted "));
  return len;
  // Decending sort start
sort:
  for (uint8_t i = temp; i <= len; i++)
  { for (uint8_t j = len; j > i; j--)
    { if (d_mod[u_i[i].ix].result < d_mod[u_i[j].ix].result)
      { //swap
        temp = u_i[j].ix;
        u_i[j].ix = u_i[i].ix;
        u_i[i].ix = temp;
      }
    }
  }
  return len;
}
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
  //  ALL DATA IN UPPER CASE
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
  s.toUpperCase();
  return s;
}

String h2s_old(String *str, uint8_t len) //padding of zero in hex
{
  //  ALL DATA IN UPPER CASE
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
    GSM.println(F("AT+CSQ"));
    String temp = GSM.readString();
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
    if ((((sig / 31.0) * 100) != 0.0) && (((sig / 31.0) * 100) <= 33.33)) {
      signaal_led(1, 0, 0);
    }
    else if ((((sig / 31.0) * 100) > 33.33) && (((sig / 31.0) * 100) <= 66.66)) {
      signaal_led(1, 1, 0);
    }
    else if ((((sig / 31.0) * 100) > 66.66) && (((sig / 31.0) * 100) <= 80)) {
      signaal_led(1, 1, 1);
    }
    Serial.print((sig / 31.0) * 100);
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
    GSM.println(F("AT+CPOWD=0"));
  wait_ok(0, 1);
  //  st = GSM.readString();
  if (!checkstring(&st, F("RDY"), 5))
  {
    //    waiit for  30 second
    wdrst();
    delay(2000);
    wdrst();
    GSM.println(F("AT"));
    wdrst();
    wait_ok(0, 1);
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
{ GSM.begin(gsmbaud);
  GSM.println(F("AT"));
  wait_ok(0, 1);
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

bool wait_ok(bool debug, bool error) //PASS
{ //  Serial.println(F("Wait OK"));
  uint32_t t = millis();
  wdrst();
  st = GSM.readString();
  //Serial.println(st);
  while (!checkstring(&st, F("OK"), 2))
  { wdrst();
    if (debug)
      Serial.println(st);
    if (checkstring(&st, F("ERROR"), 5))
      return error;
    if ((millis() - t) > 3000)
    { return false;
    }
    Serial.print(F("."));
    st = GSM.readString();
  }
  return true;
}

void setting()//to set a sever //PASS
{ // put your setup code here, to run once:
  //DISABLE SLEEP MODE
  //DISSCONNECT CALL
  GSM.begin(gsmbaud);
  GSM.println(F("ATE0"));
  wait_ok(0, 1); //wait for busy s.. string
  GSM.println(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""));
  wait_ok(0, 1);
  //  GSM.println(F("AT+SAPBR=3,1,\"APN\",\"internet\""));
  GSM.println(F("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\""));
  wait_ok(0, 1); //wait for busy s.. string
  if (!test_esp())
  { restartesp(1);
    wdrst();
    setting();
    wdrst();
  }
  return;
}

bool check_2g()
{
  Serial.println(F("AT+SAPBR=2,1"));
  GSM.println(F("AT+SAPBR=2,1"));
  if (wait_ok(0, 1))
  { if (checkstring(&st, F("PBR: "), 5))
    { //st[index] = P
      index += 3;
      Serial.println(st[index]);
      if (st[index] != '1')
      { //CONNECT GPRS AT+SAPBR=1,1
        Serial.println(st[index]);
        Serial.println(F("Connecting GPRS"));
        GSM.println(F("AT+SAPBR=3,1,\"APN\",\"internet\""));
        if (!wait_ok(0, 1)) //wait for busy s.. string
          return false;
        GSM.println(F("AT+SAPBR=1,1"));//CONNECT BARRIER
        //Check for +SAPBR: 1,1
        if (!wait_ok(0, 1))
          return false;
        return true;
      }
      return true;
    }
  }
}

int8_t POSTmod_upload_raju(int8_t i)
{ //  Serial.println(sort());
  //If no data read delete all blocks
  if (i == -2)
  { i = sort();
    if (i == -1)
      return -1;
    //    while (i >= 0)
    //    { Serial.print(i);
    //      Serial.print(F("\t-->\t"));
    //      Serial.print(u_i[i].ix);
    //      Serial.print(F("\t-->\t"));
    //      Serial.println(d_mod[u_i[i].ix].result);
    //      i--;
    //    }
    //    i = sort();
    i++;//tested Ok
  }
  //pivot
  //START
  //CALC. SPACE NEEDED FOR NEW STRING
  //CALLOC REQ. FOR NEW STRING
  //MAKE CAPTURE BASIC STRING (C)
  //APPEND STRING WITH DATA & DELETE DATA OF THAT ID
  //SEND STRING
  //CHECK REPLY - OPTIONAL

  /////////////////////////////////////////////
  //calculate total new appended string length
  //total string length < safe data string (400)

  struct send_data
  { uint8_t C_len;
    uint16_t send_len;
    uint16_t safe_send;//Safe send = 490 - C_len
    String capture ;
    String temp;
  } lu;
upload://come back after upload
  //  Serial.println(F("Grouping ....."));
  //  Serial.println(sort());
  //  Use u.capture to store temprary capture string
  //  CALLOC lu.capture
  //  lu.capture = calloc(90, 1);//90 for capture
  lu.capture.reserve(90);
  //calc C_len capture length of u.capture;

  //      *lu.capture  += F("/AdControl.aspx");//TE*u.data
  //      *lu.capture  += server_id_var;
  lu.capture  += F("?DT=0&DEV=V");//OLD
  //  String temp;
  //  temp.reserve(4);
  //  lu.temp = (String*)calloc(4, 1);
  //  if (lu.temp == NULL)
  //    return -1;
  lu.temp.reserve(4);
  //  lu.temp = String(device_id_var, HEX);

  lu.temp = String(device_id_var);
  lu.capture  += h2s(&lu.temp, 16); //length =4 CHANGE-/

  //  delete lu.temp;
  //  lu.temp = NULL;

  lu.capture  += F("&data=");
  //  Serial.println(lu.capture);//debug
  Serial.println(freeRam());
  lu.C_len = lengthof(&lu.capture);
  Serial.println(lu.C_len);//debug

  //  lu.send_len += lu.C_len;
  lu.C_len++;//for - '\'

  //NEW
  lu.send_len = 0;
  lu.safe_send = (450 - lu.C_len);//for GPRS
  //  Serial.print(F("Safe len=")); Serial.println(lu.safe_send);
  //calculate spcae neede for data string
  while ((lu.send_len <= lu.safe_send) && (i >= 0))
  { i--;
    if (i == -1) {
      i++; break;
    }
    lu.send_len += (10 + (4 * d_mod[u_i[i].ix].result));
    //    Serial.print(F("Grp i ="));
    //    Serial.println(i);
    //    Serial.print(F("send len ="));
    //    Serial.println(lu.send_len);
  }
  if ((lu.send_len > lu.safe_send) || (i == -1))
  {
    //    Serial.println(F("Going back to"));
    lu.send_len -= (10 + (4 * d_mod[u_i[i].ix].result));
    i++;
    //    Serial.print(F("Grp i ="));
    //    Serial.println(i);
    //    Serial.print(F("send len ="));
    //    Serial.println(lu.send_len);
  }
  //  Serial.println(freeRam());
  //CALLOC REQ. FOR NEW STRING
  //  u.data = (String*)realloc(lu.capture, (lu.send_len + lu.C_len));
  u.data = (String*)calloc((lu.send_len + lu.C_len + 1), 1);//+1 for null
  if (u.data == NULL)
    return -1;

  *u.data += lu.capture;
  //  Serial.println(*u.data);
  //  delete lu.capture;
  //  delete lu.capture;//added new line
  Serial.println(freeRam());
  //tested ok 5288 - 4940 = 348
  //NOW calulate length of total string required
  //total length of string = C + send_len;

  //MAKE CAPTURE BASIC STRING (C)
  //append all data with start address and no. of poll
  //use lu.send_len as terminator to 0
  //use safe_send to save i
  lu.safe_send = i;
  while (lu.send_len > 0)//pivot
  { //for 16-bit
    //NEW RESULT LIKE AS QINN
    //    Serial.println(freeRam());
    //    lu.temp = (String*)calloc(4, 1);
    //    if (lu.temp == NULL)
    //      return -1;
    Serial.println(freeRam());
    //get mod para from eeprom
    mod_read_para(u_i[i].ix + 1);//had bug solved
    lu.temp = String(mod.address, HEX);
    *u.data += h2s(&lu.temp, 16); //length =4
    //    delete lu.temp;
    //    Serial.println(freeRam());
    //    lu.temp = (String*)calloc(2, 1);
    //2-digit slave_id_var
    lu.temp = String(mod.slave, HEX);
    *u.data += h2s(&lu.temp, 8);//length =2//debug
    //2-digit function code
    lu.temp = String(mod.fun, HEX);
    *u.data += h2s(&lu.temp, 8); //length =2
    //2* no of polls resistor
    lu.temp = String(mod.registers * 2, HEX);
    *u.data += h2s(&lu.temp, 8); //length =2
    //    lu.temp = (String*)calloc(4, 1);
    for (lu.C_len = 0; lu.C_len < d_mod[u_i[i].ix].result; lu.C_len++)
    { //append data from d_mod[i].d use Clen as pointer
      //      Serial.println(*(d_mod[i].d + lu.C_len), HEX);
      lu.temp = String(*(d_mod[u_i[i].ix].d + lu.C_len), HEX);
      *u.data += h2s(&lu.temp, 16); //length =2
    }
    //    delete lu.temp;
    //    lu.temp = NULL;
    //    Serial.println(*u.data);
    lu.C_len = u_i[i].ix;
    //    Serial.println(lu.send_len);
    lu.send_len -= (10 + (4 * d_mod[lu.C_len].result));
    Serial.println(lu.send_len);
    Serial.println(freeRam());
    safe_del(&d_mod[lu.C_len].d);
    //    delete d_mod[lu.C_len].d;
    Serial.println();
    //    delete lu.temp;
    Serial.println(freeRam());
    //set u_i[i].ix to -1
    u_i[i].ix = -1;
    //set i
    i++;//i changed
  }
  *u.data += F("\"");
  //recover i
  i = lu.safe_send;
  //  Serial.println(i);
  //  Serial.println(*u.data);
  ///OK TESTED
  //  delete lu.temp;//caused a problem
  Serial.println(freeRam());
  //  GSM.begin(gsmbaud);
  lu.C_len = 0;
  if (!upload_data_gateway() && lu.C_len == 0)
  { //retry one more time use lu.C_len
    Serial.println(F("Retrying"));
    lu.C_len = 1;
    upload_data_gateway();
  }
  //  if fail retry one time else goto next itration
  Serial.println(F("Deleting Data"));
  Serial.println(freeRam());
  delete u.data;//delete data
  u.data = NULL;
  Serial.println(freeRam());
  Serial.println(i);
  //GO BACK TO LOOP FOR ITERATION UNTILL ALL DATA SEND
  //cause stack overflow
  if (u_i[0].ix != -1) {
    return i;
  }
  else
  { Serial.println(F("ALL Sent"));
    signaal_led(0, 0, 0);
    return -1;
  }

  //  while (i >= 0)
  //  { send_len += (10 + (4 * mod_data[u_i[i].ix].result));
  //    Serial.println(send_len);
  //    if (send_len >= safe_send)
  //    { //go back
  //      i++;
  //      send_len -= (10 + (4 * mod_data[u_i[i].ix].result));
  //      //Frame len under safe zone is now ready
  //      //got index to be read
  //      //delete data and delete index when read - check ram
  //      //append and make string - check ram
  //      //upload that string - by CBR
  //      //after upload recurse this function by returning
  //    }
  //    i--;
  //  }
}

bool POSTupload_data_gateway()
{
  //  signaal_led();
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
  { Serial.println(F("2g fail"));
    n_fail++;
    internet = false;
    s = 4;//N
    goto debug;
  }
  //  Timer1.detachInterrupt();
  Timer1.initialize(100000);//100ms
  Timer1.attachInterrupt(UPLOAD_BLINK);
  s = 2;
  //  Serial.println(F("HTTPTERM"));
  GSM.println(F("AT+HTTPTERM"));
  if (!wait_ok(0, 1))
    goto debug;
  //  Serial.println(F("HTTPINIT"));
  GSM.println(F("AT+HTTPINIT"));
  if (!wait_ok(0, 1))
    goto debug;
  wdrst();
  //  st = F("AT+HTTPPARA=\"CID\",1");
  //  GSM.println(st);
  //  Serial.println(st);
  //  //  check for ok
  //  if (!wait_ok(1, 1))//error returned as true
  //    goto debug;
  //  AT+HTTPPARA="URL","http://103.233.79.197:8000/ADSmart.asmx/Demo1?"
  st = F("AT+HTTPPARA=\"URL\",\"http://");
  st += ip_var; //IP1
  st += F(":");
  st += port_var; //PORT
  st += F("/ADSmart.asmx/Demo1"); //OLD-Demo1-V0011//10
  st += F("\"");//worked fine
  GSM.println(st);
  //  GSM.println(F("AT+HTTPPARA=\"URL\",\"http://") + ip_var + F(":") + port_var + F("/ADSmart.asmx/Demo1?") + F("\""));
  Serial.println(st);
  //  check for ok
  if (!wait_ok(0, 1))
    goto debug;
  st = F("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"");//VVI
  GSM.println(st);
  Serial.println(st);
  //  check for ok
  if (!wait_ok(1, 1))//error returned as true
    goto debug;

  //TEST - MULTIPLE SERIAL WRITE SUPPORTED
  u.data = (String*)calloc(1000, 1);
  //  *u.data = F("DT=0&DEV=V0020&data=006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
  *u.data = F("DT=0&DEV=V0020&data=");
  //  *u.data += F("\"");
  //  AT+HTTPDATA=<u.datalen>,<timeout> (exact Bytes,10000)
  st = F("AT+HTTPDATA=10388");
  //  st += lengthof(u.data);
  st += F(",15000");//10sec timeout DEFAULT
  Serial.println(st);
  GSM.println(st);
  Serial.println(GSM.readString());
  //  check for DOWNLOAD
  //  print data
  Serial.print(*u.data);//ONLY parameter of data=<parameter>
  GSM.print(*u.data);
  //  *u.data = F("FFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD0EFF0103800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000C85B4D45C83E4D45000000000F3C020394000000004FE847BB000000003E1DBF7592F02193433E4366EC1B4310E328424700000000A86B46F9000000004050BF7592F0219348314366DA13431000000000118346FA00000000368FBF7592F0219339B3436625F543110000000085B446F900000000437ABF7592F0219347D64366C44A4310000000006E464B2000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
  for (int i = 0; i < 40; i++)//258*28=7224
  { *u.data = F("006403037C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000A34366F9BB4365F96843650EC6436600000000000000000000000000000000C0D642477F694DDABB984DDD");
    Serial.print(*u.data);//ONLY parameter of data=<parameter>
    GSM.print(*u.data);
  }
  for (uint32_t i = 0; i < 58; i++)//1
  {
    Serial.print(F("A"));//ONLY parameter of data=<parameter>
    GSM.print(F("A"));
  }
  //  Serial.println(GSM.readString());
  if (!wait_ok(1, 0))//if error return 0
    goto debug;
  /////////////////////////////////////////////////////////////////// copied
  Serial.println();
  Serial.println(F("Closing HTTP"));
  GSM.println(F("AT+HTTPACTION=1"));//POST REQUEST
  if (wait_ok(1, 0)) //if error return 0
  { if (state == false)
    { Timer1.initialize(50000);
      Timer1.attachInterrupt(UPLOAD_BLINK_WAIT_on);
    }
    if (state == true)
    { Timer1.initialize(400000);
      Timer1.attachInterrupt(UPLOAD_BLINK_WAIT_off);
    }
    uint32_t t = millis() / 100;
    while (!checkstring(&st, F("TION:"), 5))//CHECK FOR +HTTPACTION
    {
      Serial.println(st);
      st = GSM.readString();
      if (((millis() / 100) - t) > 250) //15SEC 5000/100=50
      { Serial.println(F("TIME OUT"));
        //        GSM.println(F("AT+SAPBR=0,1"));//CHANGE+
        //        if (!wait_ok(0,1))
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
    GSM.println(F("AT+HTTPREAD"));
    if (wait_ok(0, 1))
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
  }
debug:
  s = 0;
  Serial.println(F("UPLOAD FAIL"));
  Timer1.detachInterrupt();
  fail(43);
  Serial.println(st);
  GSM.println(F("AT+HTTPREAD"));//change-
  Serial.println(GSM.readString());//change-
  return false;
}

bool upload_data_gateway()
{
  //  signaal_led();
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
  { Serial.println(F("2g fail"));
    n_fail++;
    internet = false;
    s = 4;//N
    goto debug;
  }
  //  Timer1.detachInterrupt();
  Timer1.initialize(100000);//100ms
  Timer1.attachInterrupt(UPLOAD_BLINK);
  s = 2;
  //  Serial.println(F("HTTPTERM"));
  GSM.println(F("AT+HTTPTERM"));
  if (!wait_ok(0, 1))
    goto debug;
  //  Serial.println(F("HTTPINIT"));
  GSM.println(F("AT+HTTPINIT"));
  if (!wait_ok(0, 1))
    goto debug;
  wdrst();
  //  AT+HTTPPARA="URL","http://103.233.79.197:8000/ADSmart.asmx/Demo1?"
  //  AT+HTTPDATA=<u.datalen>,<timeout> (Bytes,10000)
  //  print data
  Serial.println(*u.data);//ONLY parameter of data=<parameter>
  GSM.println(*u.data);
  if (!wait_ok(0, 1))
    goto debug;
  /////////////////////////////////////////////////////////////////// copied
  //  Serial.println(F("Closing HTTP"));
  GSM.println(F("AT+HTTPACTION=0"));//GET REQUEST
  if (wait_ok(0, 1))
  { if (state == false)
    { Timer1.initialize(50000);
      Timer1.attachInterrupt(UPLOAD_BLINK_WAIT_on);
    }
    if (state == true)
    { Timer1.initialize(400000);
      Timer1.attachInterrupt(UPLOAD_BLINK_WAIT_off);
    }
    uint32_t t = millis() / 100;
    while (!checkstring(&st, F("TION:"), 5))//CHECK FOR +HTTPACTION
    {
      //      Serial.println(st);
      st = GSM.readString();
      if (((millis() / 100) - t) > 150) //15SEC 5000/100=50
      { Serial.println(F("TIME OUT"));
        //        GSM.println(F("AT+SAPBR=0,1"));//CHANGE+
        //        if (!wait_ok(0,1))
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
    GSM.println(F("AT+HTTPREAD"));
    if (wait_ok(0, 1))
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
//bool upload_raju(String *d)//new capture then pass
//{
//  uint32_t upload_time = millis();
//  //  Serial.println(F("UPLOAD_raju"));
//  //connect and check server
//  //blink upload led to tell connecting
//  s = 1;
//  Timer1.initialize(400000);//100ms
//  Timer1.attachInterrupt(UPLOAD_BLINK);
//  signaal();
//  //Check gprs connection
//  if (!check_2g())
//  { n_fail++;
//    internet = false;
//    s = 4;//N
//    goto debug;
//  }
//  //  Timer1.detachInterrupt();
//  Timer1.initialize(100000);//100ms
//  Timer1.attachInterrupt(UPLOAD_BLINK);
//  s = 2;
//  //  Serial.println(F("HTTPTERM"));
//  GSM.println(F("AT+HTTPTERM"));
//  if (!wait_ok(0,1))
//    goto debug;
//  //  Serial.println(F("HTTPINIT"));
//  GSM.println(F("AT+HTTPINIT"));
//  if (!wait_ok(0,1))
//    goto debug;
//  wdrst();
//  st = F("AT+HTTPPARA=\"URL\",\"http://");
//  st += ip_var; //IP1//CHANGE-
//  //  st += F("103.233.79.197");
//  st += F(":");
//  st += port_var;//PORT//CHANGE-
//  //  st += F("8000");
//  st += F("/ADSmart.asmx/Demo1");//OLD
//  //      st += F("/AdControl.aspx");//TEST
//  //      st += server_id_var;
//  st += F("?DT=0&DEV=SGA0001");//OLD
//  //      st += F("?DT=0&DEV=V");//TEST
//  //      //cid
//  //      String temp = String(client_id_var.toInt(), HEX);
//  //      st += h2s(&temp, 16); //length =4
//  //      //Device id
//  //      st += F("SGR");
//  //  /CHANGE-
//  //  String temp;
//  //  temp.reserve(4);
//  //  temp = String(device_id_var, HEX);
//  //  st += h2s(&temp, 16); //length =4 CHANGE-/
//  //      Serial.println(st);
//  st += F("&DATA=");
//  //6DIGIT DUMMY BY RAJU
//  //      st += F("&DATA=ABCDEF");
//  //I AM PUTTING 4-DIGIT CLIENT ID and 2-digit dummy
//  //  Serial.println(*d);
//  st += *d;
//  st += F("\"");
//  //  Serial.println(st);
//  GSM.println(st);
//  if (!wait_ok(0,1))
//    goto debug;
//  /////////////////////////////////////////////////////////////////// copied
//  //  Serial.println(F("Closing HTTP"));
//  GSM.println(F("AT+HTTPACTION=0"));
//  if (wait_ok(0,1))
//  {
//    uint32_t t = millis() / 100;
//    while (!checkstring(&st, F("TION:"), 5))//CHECK FOR +HTTPACTION
//    {
//      //      Serial.println(st);
//      st = GSM.readString();
//      if (((millis() / 100) - t) > 100) //10SEC=5000/100=50
//      { Serial.println(F("TIME OUT"));
//        //        GSM.println(F("AT+SAPBR=0,1"));//CHANGE+
//        //        if (!wait_ok(0,1))
//        //          return false;
//        //        internet = false;
//        s = 3;
//        //        printtimer = millis();
//        goto debug;
//      }
//      //      delay(100);
//    }
//    //+HTTPACTION:0,200,88
//    s = 6;
//    if (!checkstring(&st, F("200,"), 4))
//      goto debug;
//    Serial.println(F("LINK OK"));
//    internet = true;
//    GSM.println(F("AT+HTTPREAD"));
//    if (wait_ok(0,1))
//    {
//      uint32_t t = millis();
//      while (!checkstring(&st, F("READ:"), 5))//CHECK FOR +HTTPREAD: LEN
//      { //        print_page();
//      }
//      if (!checkstring(&st, F("ACK"), 3))//UPLOAD ACKNOWLEDGEMENT
//        goto debug;
//      Serial.println(F("UPLOAD DONE"));
//      Timer1.detachInterrupt();
//      digitalWrite(43, 1);
//      Serial.println(millis() - upload_time);
//      s = 7;
//      return true;
//      //      Serial.println(st[index]);
//    }
//debug:
//    s = 0;
//    Serial.println(F("UPLOAD FAIL"));
//    Timer1.detachInterrupt();
//    fail(43);
//    Serial.println(st);
//    return false;
//  }
//}

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
  //  wdt_enable(WDTO_4S);//change- disabled to test
  boot();
  //  delay(200);
  Serial.begin(19200);
  Serial.println(freeRam());
  Serial.println(F("F/w: V-1.2"));//1 for post
  Serial.println(F("F/w Innovated by: SEPS MONITORING PVT LTD."));
  port_var.reserve(4);
  ip_var.reserve(19);
  st.reserve(400);//usefull to avoid fragment

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
  GSM.begin(gsmbaud);
  GSM.setTimeout(200);//VVI FOR CODE OPTIMIZATION
  GSM.println(F("AT"));
  Serial.println(GSM.readString());
  //RS485 SETUP
  pinMode(MAX485_DE, OUTPUT);
  begin_rs485();//rs485.begin(baud, NONE);
  setting();
  wdrst();
  //timer reset
  modbus_timer = millis() / 100;
  tu = millis();//timer to upload data
  Serial.println(F("SETUP DONE"));
  Serial.println(freeRam());
upload_test:
  POSTupload_data_gateway();
  delete u.data;
  //  delay(30000);
  delay(1000000);//change-
  //  goto upload_test;
}

void read_data()
{ uint8_t j, result;//j- no. of attempts

  for (uint8_t i = 0; i < nBlock; i++)
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
      // Callbacks allow us to configure the RS485 transceiver correctly
      node.preTransmission(preTransmission);
      node.postTransmission(postTransmission);
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
            c.d = *(d_mod[i].d + (j + 1));
            c.d = c.d << 16;
            c.d = c.d | *(d_mod[i].d + j);
            //            c.d = d32;
            //      Serial.println(data[j], HEX);
            //      Serial.print(u.d, HEX);
            Serial.print(c.d, HEX);
            Serial.print(F("="));
            Serial.println(c.f);
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
}//SAVES RAM 4B


int8_t mod_upload_raju(int8_t i)
{ //  Serial.println(sort());
  //If no data read delete all blocks
  if (i == -2)
  { i = sort();
    if (i == -1)
      return -1;
    //    while (i >= 0)
    //    { Serial.print(i);
    //      Serial.print(F("\t-->\t"));
    //      Serial.print(u_i[i].ix);
    //      Serial.print(F("\t-->\t"));
    //      Serial.println(d_mod[u_i[i].ix].result);
    //      i--;
    //    }
    //    i = sort();
    i++;//tested Ok
  }
  //pivot
  //START
  //CALC. SPACE NEEDED FOR NEW STRING
  //CALLOC REQ. FOR NEW STRING
  //MAKE CAPTURE BASIC STRING (C)
  //APPEND STRING WITH DATA & DELETE DATA OF THAT ID
  //SEND STRING
  //CHECK REPLY - OPTIONAL

  /////////////////////////////////////////////
  //calculate total new appended string length
  //total string length < safe data string (400)

  struct send_data
  { uint8_t C_len;
    uint16_t send_len;
    uint16_t safe_send;//Safe send = 490 - C_len
    String capture ;
    String temp;
  } lu;
upload://come back after upload
  //  Serial.println(F("Grouping ....."));
  //  Serial.println(sort());
  //  Use u.capture to store temprary capture string
  //  CALLOC lu.capture
  //  lu.capture = calloc(90, 1);//90 for capture
  lu.capture.reserve(90);
  //calc C_len capture length of u.capture;
  lu.capture  = F("AT+HTTPPARA=\"URL\",\"http://");
  lu.capture  += ip_var; //IP1
  lu.capture  += F(":");
  lu.capture  += port_var;//PORT
  lu.capture  += F("/ADSmart.asmx/Demo1");//OLD-Demo1-V0011
  //      *lu.capture  += F("/AdControl.aspx");//TE*u.data
  //      *lu.capture  += server_id_var;
  lu.capture  += F("?DT=0&DEV=V");//OLD
  //  String temp;
  //  temp.reserve(4);
  //  lu.temp = (String*)calloc(4, 1);
  //  if (lu.temp == NULL)
  //    return -1;
  lu.temp.reserve(4);
  //  lu.temp = String(device_id_var, HEX);

  lu.temp = String(device_id_var);
  lu.capture  += h2s(&lu.temp, 16); //length =4 CHANGE-/

  //  delete lu.temp;
  //  lu.temp = NULL;

  lu.capture  += F("&data=");
  //  Serial.println(lu.capture);//debug
  Serial.println(freeRam());
  lu.C_len = lengthof(&lu.capture);
  Serial.println(lu.C_len);//debug

  //  lu.send_len += lu.C_len;
  lu.C_len++;//for - '\'

  //NEW
  lu.send_len = 0;
  lu.safe_send = (450 - lu.C_len);//for GPRS
  //  Serial.print(F("Safe len=")); Serial.println(lu.safe_send);
  //calculate spcae neede for data string
  while ((lu.send_len <= lu.safe_send) && (i >= 0))
  { i--;
    if (i == -1) {
      i++; break;
    }
    lu.send_len += (10 + (4 * d_mod[u_i[i].ix].result));
    //    Serial.print(F("Grp i ="));
    //    Serial.println(i);
    //    Serial.print(F("send len ="));
    //    Serial.println(lu.send_len);
  }
  if ((lu.send_len > lu.safe_send) || (i == -1))
  {
    //    Serial.println(F("Going back to"));
    lu.send_len -= (10 + (4 * d_mod[u_i[i].ix].result));
    i++;
    //    Serial.print(F("Grp i ="));
    //    Serial.println(i);
    //    Serial.print(F("send len ="));
    //    Serial.println(lu.send_len);
  }
  //  Serial.println(freeRam());
  //CALLOC REQ. FOR NEW STRING
  //  u.data = (String*)realloc(lu.capture, (lu.send_len + lu.C_len));
  u.data = (String*)calloc((lu.send_len + lu.C_len + 1), 1);//+1 for null
  if (u.data == NULL)
    return -1;

  *u.data += lu.capture;
  //  Serial.println(*u.data);
  //  delete lu.capture;
  //  delete lu.capture;//added new line
  Serial.println(freeRam());
  //tested ok 5288 - 4940 = 348
  //NOW calulate length of total string required
  //total length of string = C + send_len;

  //MAKE CAPTURE BASIC STRING (C)
  //append all data with start address and no. of poll
  //use lu.send_len as terminator to 0
  //use safe_send to save i
  lu.safe_send = i;
  while (lu.send_len > 0)//pivot
  { //for 16-bit
    //NEW RESULT LIKE AS QINN
    //    Serial.println(freeRam());
    //    lu.temp = (String*)calloc(4, 1);
    //    if (lu.temp == NULL)
    //      return -1;
    Serial.println(freeRam());
    //get mod para from eeprom
    mod_read_para(u_i[i].ix + 1);//had bug solved
    lu.temp = String(mod.address, HEX);
    *u.data += h2s(&lu.temp, 16); //length =4
    //    delete lu.temp;
    //    Serial.println(freeRam());
    //    lu.temp = (String*)calloc(2, 1);
    //2-digit slave_id_var
    lu.temp = String(mod.slave, HEX);
    *u.data += h2s(&lu.temp, 8);//length =2//debug
    //2-digit function code
    lu.temp = String(mod.fun, HEX);
    *u.data += h2s(&lu.temp, 8); //length =2
    //2* no of polls resistor
    lu.temp = String(mod.registers * 2, HEX);
    *u.data += h2s(&lu.temp, 8); //length =2
    //    lu.temp = (String*)calloc(4, 1);
    for (lu.C_len = 0; lu.C_len < d_mod[u_i[i].ix].result; lu.C_len++)
    { //append data from d_mod[i].d use Clen as pointer
      //      Serial.println(*(d_mod[i].d + lu.C_len), HEX);
      lu.temp = String(*(d_mod[u_i[i].ix].d + lu.C_len), HEX);
      *u.data += h2s(&lu.temp, 16); //length =2
    }
    //    delete lu.temp;
    //    lu.temp = NULL;
    //    Serial.println(*u.data);
    lu.C_len = u_i[i].ix;
    //    Serial.println(lu.send_len);
    lu.send_len -= (10 + (4 * d_mod[lu.C_len].result));
    Serial.println(lu.send_len);
    Serial.println(freeRam());
    safe_del(&d_mod[lu.C_len].d);
    //    delete d_mod[lu.C_len].d;
    Serial.println();
    //    delete lu.temp;
    Serial.println(freeRam());
    //set u_i[i].ix to -1
    u_i[i].ix = -1;
    //set i
    i++;//i changed
  }
  *u.data += F("\"");
  //recover i
  i = lu.safe_send;
  //  Serial.println(i);
  //  Serial.println(*u.data);
  ///OK TESTED
  //  delete lu.temp;//caused a problem
  Serial.println(freeRam());
  //  GSM.begin(gsmbaud);
  lu.C_len = 0;
  if (!upload_data_gateway() && lu.C_len == 0)
  { //retry one more time use lu.C_len
    Serial.println(F("Retrying"));
    lu.C_len = 1;
    upload_data_gateway();
  }
  //  if fail retry one time else goto next itration
  Serial.println(F("Deleting Data"));
  Serial.println(freeRam());
  delete u.data;//delete data
  u.data = NULL;
  Serial.println(freeRam());
  Serial.println(i);
  //GO BACK TO LOOP FOR ITERATION UNTILL ALL DATA SEND
  //cause stack overflow
  if (u_i[0].ix != -1) {
    return i;
  }
  else
  { Serial.println(F("ALL Sent"));
    signaal_led(0, 0, 0);
    return -1;
  }

  //  while (i >= 0)
  //  { send_len += (10 + (4 * mod_data[u_i[i].ix].result));
  //    Serial.println(send_len);
  //    if (send_len >= safe_send)
  //    { //go back
  //      i++;
  //      send_len -= (10 + (4 * mod_data[u_i[i].ix].result));
  //      //Frame len under safe zone is now ready
  //      //got index to be read
  //      //delete data and delete index when read - check ram
  //      //append and make string - check ram
  //      //upload that string - by CBR
  //      //after upload recurse this function by returning
  //    }
  //    i--;
  //  }
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
      Serial.println(freeRam());
      //begin upload
      int8_t i = mod_upload_raju(-2);
      while (i != -1)
      { i = mod_upload_raju(i);
      }
      //        Serial.println(F("No data read"));
      //      //make ram free
      //      for (i = 0; i < nBlock; i++)
      //      { delete d_mod[i].d;
      //      }
      goto done;
    }
    delay(10000000);//change-
  }
}
