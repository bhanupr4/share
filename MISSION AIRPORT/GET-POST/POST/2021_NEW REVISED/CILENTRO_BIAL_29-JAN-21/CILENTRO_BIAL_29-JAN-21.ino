#include<EEPROM.h>
#include <ModbusMaster.h>
#include<avr/wdt.h>
#include <BhanuGSM.h>

//MEGA
#include<TimerOne.h>

/*
1.> 2- SERVER
2.> MULTIPLE FORMAT
*/

//CONFIG
#define WDT 1UL //IN SEC
#define WDTms WDT * 1000UL
#define WDTus WDTms * 1000UL

#define SHOW_SERIAL 1
//#define Serial SerialUSB //DEBUG OR PROGRAM SERIAL
#define RS485 Serial3
#define GSM_SERIAL Serial2 //gsm
#define DGSM GSM_SERIAL

///////////////////////////// PINS ///////////////////////
//MAX3485ESA+
#define MAX485_DE 49 //3//-for big WIFI//49//for airport

//GSM
#define RST PA8 //PB5//6//NEW

//LED
#define sig0        44
#define sig1        45
#define sig2        46

#define PIN_GREEN   43
#define PIN_RED     42
#define PIN_YELLOW  47
#define L_GREEN     PIN_GREEN
#define L_RED       PIN_RED
#define L_YELLOW    PIN_YELLOW
#define UPLOAD      L_GREEN
#define APP         L_YELLOW
#define MODBUS      L_RED
////////////////////////////////////////////////////

//LED
#define ON HIGH
#define OFF LOW

#define START_add_ID 1
#define nBlock 32 * 2 //number of db block

#define RATIO_100 100 //ms ratio
#define UPLOAD_MULT ((1000 / RATIO_100) * 5)
#define UPLOAD_OFFSET 800 / RATIO_100 //ms offset//0.5sec OFFSET

#define STATUS_UPDATE_TIME 5

//EEPROM MAP
#define last_ID (((nBlock * sizeof(modbus)) + START_add_ID) - sizeof(modbus)) //MAY BE VVI

//START FROM last_ID + sizeof(modbus)
//EEPROM INT-8    START_ADD
#define DATA_START (last_ID + sizeof(modbus))
#define MOD_BAUD DATA_START   //*100 BAUD
#define PARITY DATA_START + 2 //1-N, 2-O, 3-E
#define MOD_FUN DATA_START + 4
#define MOD_RETRY DATA_START + 8
#define UPLOAD_d DATA_START + 10 //*5sec
#define U_RETRY DATA_START + 12
//EEPROM INT-16   START_ADD
//SERVER
#define CLIENT_ID DATA_START + 17
#define DEVICE_ID DATA_START + 20
#define PORT DATA_START + 23
//EEPROM STRING   START_ADD   END_ADD
#define S_HOST DATA_START + 27
#define E_HOST DATA_START + 127
#define S_METHOD DATA_START + 128
#define E_METHOD DATA_START + 178
#define S_CONTENT DATA_START + 179
#define E_CONTENT DATA_START + 239

#define S_HOST1 DATA_START + 240
#define E_HOST1 DATA_START + 340
#define S_METHOD1 DATA_START + 341
#define E_METHOD1 DATA_START + 391
#define S_CONTENT1 DATA_START + 392
#define E_CONTENT1 DATA_START + 452

//GSM
#define gsmbaud 460800 //115200//19200
#define tout1 100
#define toutHTTPDATA 5000
#define toutSTATUS 20000 //8000
#define toutHTTP 20000
#define NOT_INTERNET_COUNT 10 //20
#define DEBUG 0b00110001      //on error quick get error
//#define error       0b00110000
#define nZF1Eo1 0b10010001
#define ZF1Eo1 0b00010001
#define nZF1E0 0b10010000
#define ZF3oEo1 0b00111001
#define ZF3Eo1 0b00110001
#define ZF2Eo1 0b00010001
#define ZF0Eo1 0b00000001
#define ZF3oE0 0b00111000
#define ZF3E0 0b00110000
#define ZF2E0 0b00100000
#define ZF1E0 0b00010000
#define ZF0E0 0b00000000
#define nZF3oEo1 0b10111001
#define nZF3Eo1 0b10110001
#define nZF3oE0 0b10111000
#define nZF3E0 0b10110000
#define GSMtest 0b00001111
#define GPRStest 0b00001000
#define SIMtest 0b00000010
#define NWtest 0b00000100
#define CCLKtest 0b00010000
#define HouUnix 0b00111100
#define MinUnix 0b00111110
#define SecUnix 0b00111111
#define SAP1APN 0b00000010
#define CSTTAPN 0b00000100
#define BOTHAPN 0b00000110
#define oSAP1APN 0b00000011
#define oCSTTAPN 0b00000101
#define oBOTHAPN 0b00000111
#define GSMHeal 0b00000011  //at + sim
#define WiFiHeal 0b00000001 //at
#define SwRST 0b00000000
#define SigPct 0b00000000
#define Sigusdb 0b00000001
#define Sigsdb 0b10000001

#define ONE_TIME 0x8000
#define ATV1 0x0001
#define CREG0 0x0002
#define CSDT1 0x0004
#define CSCLK0 0x0008
#define CMGF1 0x0010
#define SLED 0x0020
#define CMGDAall 0x0040
#define CNMI 0x0080 //VVI FOR SMS//New SMS Message Indications
#define ATE0 0x0800

#define ONE_TIME_BASIC 0x88BF //(ONE_TIME|ATV1|CREG0|CSDT1|CSCLK0|CMGF1|SLED|CNMI|ATE0)

#define nMP 5
#define nMP1 (nMP - 1)
#define nMP2 (nMP - 2)
#define nMP3 (nMP - 3)
#define nMP4 (nMP - 4)
#define nMP5 (nMP - 5)

#define TYPE_ABP 0
#define TYPE_QINN 1
#define TYPE_JSON 2

#define SERVER 0
#define SERVER1 1

//TIM_TypeDef *LED_tim = TIM2;
//HardwareTimer *LED_BLINK = new HardwareTimer(LED_tim);

//TIM_TypeDef *cmd = TIM1;
//HardwareTimer *Command = new HardwareTimer(cmd);

BhanuGSM GSM(GSM_SERIAL, Serial); //(GSM,Serial)

// instantiate ModbusMaster object
ModbusMaster node;

//global variable
int8_t mod_parity_var;
int16_t mod_baud_100;
uint8_t get_upload_var;
uint8_t u_retry_var;
uint8_t mod_retry_var;
uint16_t client_id_var;
uint16_t device_id_var;
//for conversion hex to float
// union {
//   uint32_t d;
//   float f;
// } u;

// FOR MODBUS EEPROM TABLE
struct modbus
{
  //
  byte ID; //ID FOR MOD PARAMETER
  uint8_t slave;
  uint8_t fun;
  uint16_t address;
  uint8_t registers;
} mod;

// struct upload_index {
//   int8_t ix = -1;
// } u_i[nBlock];

struct modDATA
{
  // can be read from eeprom
  int8_t result = -1; //for status of read
  uint16_t *d;        //dynamic
} d_mod[nBlock];

// BASIC EEPROM VARIABLE
// uint8_t mod_parity_var = 0;
// uint8_t mod_baud_var = 96;
// uint8_t mod_retry_var = 3;

uint8_t data_count = 0;

int8_t notGPRS = NOT_INTERNET_COUNT;

volatile uint8_t state = false;
volatile bool LED_state = false;

//for internal eeprom
uint16_t add_ID = START_add_ID; //VVI BUT CAN BE LOCALLY USED
// uint16_t last_ID = ((nBlock*sizeof(modbus) + START_add_ID) - sizeof(modbus));//MAY BE VVI

void UPLOAD_BLINK_WAIT_off(void);
void UPLOAD_BLINK_WAIT_on(void);
void GSM_Heal();

//led
void fail(uint8_t pin)
{
  if (pin == UPLOAD)
  {
    LED_BLINK->detachInterrupt();
    // power_timer1_disable();
    if (!(state & 0x01))
    {
      digitalWrite(pin, OFF);
    }
    state |= 0x01; //true;//future
  }
  //  delay(1000);//change+//test+
  for (uint8_t i = 1; i <= 4; i++)
  { //2- 0 1 0 1//1- 1 0 1 0
    wdt_reset();
    //    Serial.print(i % 2);
    digitalWrite(pin, (i % 2));
    if (i % 4 != 0)
    {
      delay(100);
    }
  }
  delay(50);
  //  Serial.println();
}

void MOD_failed()
{
  //ALTER OR CHANGE sate.bit-1 // use XOR operation
  state ^= 0x02;
  digitalWrite(MODBUS, ((state >> 1) & 0x01));
}

void MOD_fail_reset()
{
  LED_BLINK->detachInterrupt();
  // Turn off MOD led
  digitalWrite(MODBUS, OFF);
}

void LED_TOGGLE_IT()
{
  LED_state == true ? LED_state = false : LED_state = true;
  digitalWrite(L_RED, LED_state);
  digitalWrite(L_GREEN, LED_state);
  digitalWrite(L_YELLOW, LED_state);
}

void LED(uint8_t state)
{
  digitalWrite(L_RED, state);
  digitalWrite(L_GREEN, state);
  digitalWrite(L_YELLOW, state);
}

void UPLOAD_BLINK()
{
  //  state = !state;
  state ^= 0x01;
  //use direct pin write
  digitalWrite(UPLOAD, state); //uses 56 cycle
}

static void upload_1()
{
  digitalWrite(UPLOAD, ON); //AT passed
  state &= 0xFE;            //false;
}

void UPLOAD_BLINK_WAIT_off(void)
{
  if ((state & 0x01) == 0x01)
  {
    digitalWrite(UPLOAD, OFF);
  }
  state &= 0xFE; //false;
  LED_BLINK->setOverflow(50000UL, MICROSEC_FORMAT);
  LED_BLINK->attachInterrupt(UPLOAD_BLINK_WAIT_on);
}

void UPLOAD_BLINK_WAIT_on(void)
{
  if ((state & 0x01) == 0)
  {
    digitalWrite(UPLOAD, ON);
  }
  state |= 0x01; //true;
  LED_BLINK->setOverflow(400000UL, MICROSEC_FORMAT);
  LED_BLINK->attachInterrupt(UPLOAD_BLINK_WAIT_off);
}

uint8_t get_upload()
{ //FREQUENTLY
  uint8_t u = EEPROM.read(UPLOAD_d);
  //  Serial.print(u * 5);
  //  Serial.println(F("<--U(s)"));
  if (u != 0 || u >= 60)
    return u;
  else
    return 5; // * 5-sec//25sec
}

uint8_t get_u_retry() //MULTIPLE TIMES
{
  uint8_t u_retry_var = EEPROM.read(U_RETRY);
  //  Serial.print(u_retry_var);
  //  Serial.println(F("<--UPLOAD RETRY"));
  if (u_retry_var <= 10 && u_retry_var != 0)
    return u_retry_var;
  else
    return 5;
}

uint32_t get_mod_baud() //ONE TIME
{
  uint8_t temp = EEPROM.read(MOD_BAUD);

  // Serial.print(temp);
  // Serial.println(F("<--MOD baud"));
  if (temp == 1)
    return 48;
  //  else if (temp == 2)
  //    return 96;
  else if (temp == 3)
    return 192;
  else if (temp == 4)
    return 384;
  else if (temp == 5)
    return 576;
  else if (temp == 6)
    return 1152;
  else if (temp == 7)
    return 2304;
  else
    return 96;
}

uint8_t get_mod_parity() //ONE TIME
{
  uint8_t mod_parity_var = EEPROM.read(PARITY);
  // mod_parity_var++;

  //  Serial.print(mod_parity_var);
  //  Serial.print(F("<--P "));
  //  if (mod_parity_var == 1)
  //    Serial.println(F("N"));
  //  else if (mod_parity_var == 2)
  //    Serial.println(F("O"));
  //  else if (mod_parity_var == 3)
  //    Serial.println(F("E"));

  if (mod_parity_var < 4 && mod_parity_var > 0)
    return mod_parity_var;
  else
    return 1;
}

// uint8_t get_mod_fun()//MULTIPLE TIME
// { uint8_t mod_fun_var = EEPROM.read(MOD_FUN);

//   //  Serial.print(mod_fun_var);
//   //  Serial.println(F("<--MODfun"));
//   if (mod_fun_var == 3 || mod_fun_var == 4)
//     return mod_fun_var;
//   else
//     return 3;
// }

uint8_t get_mod_retry() //MULTIPLE TIMES
{
  uint8_t mod_retry_var = EEPROM.read(MOD_RETRY);

  //  Serial.print(mod_retry_var);
  //  Serial.println(F("<--MOD RETRY"));
  if (mod_retry_var <= 20 && mod_retry_var != 0)
    return mod_retry_var;
  else
    return 4;
}

uint16_t get_client_id()
{
  //  Serial.print(client_id_var);
  //  Serial.println(F("<--CID"));
  return Read16Int(CLIENT_ID);
}

uint16_t get_device_id()
{
  //  Serial.print(device_id_var);
  //  Serial.println(F("<--DID"));
  return Read16Int(DEVICE_ID);
}

bool SIM()
{
  uint32_t sim_wait = millis() / 1000;
  // uint16_t _sim_wait16;
  // Serial.println(GSM.GetCmd(F("AT+CSPN?"), nZF3E0, tout1 + 1000));
//  Serial.println(GSM.CheckCmd(F("AT+CSPN?"), nZF3E0, tout1 + 1000));//TEST
  // Serial.println(GSM.Test(SIMtest)&SIMtest);
  //wait 5-sec
  sim_wait = millis() / 1000;
  // _sim_wait16 = millis() / 1000;

  // Serial.print(F("wait: "));
  // Serial.println(sim_wait);
  // Serial.print(F("\twait16: "));
  // Serial.println(_sim_wait16);
  while (((millis() / 1000) - sim_wait) < 7)//not entering in loop
  { //wait 7-sec time for sim detection
    //CHECK SIM
    Serial.print(F("."));
    wdt_reset();
    if ((GSM.Test(SIMtest)&SIMtest)>0)//bug
    {
      return 1;
    }
  }
  return 0;
}

bool HTTPInit()
{
  //  AT+HTTPTERM
  //  GSM.CheckCmd(ReadeepString(AT_) + ReadeepString(HTTP) + ReadeepString(TERM), ZF3E0, tout1);
  //  GSM.CheckCmd(&ReadeepString(AT_), ZF3E0, tout1);
  GSM.CheckCmd(F("AT+HTTPTERM"), ZF3E0, tout1);

  //  AT+HTTPINIT
  //  return GSM.CheckCmd(ReadeepString(AT_) + ReadeepString(HTTP) + ReadeepString(INIT), ZF3E0, tout1);
  //  return GSM.CheckCmd(&ReadeepString(AT_), ZF3E0, tout1);
  return GSM.CheckCmd(F("AT+HTTPINIT"), ZF3E0, tout1);
}

uint8_t HTTPsetup(uint8_t server)
{
  String Host;
  if (server == SERVER1)
    Host = ReadeepString(S_HOST1);
  else
    Host = ReadeepString(S_HOST);
  if (Host == nullptr)
  {
    return 0;
  }

  //CHECK GSM TIME
  // AT+CNTP=\"time.google.com\",22
  // Serial.println(GSM.GetCmd(F("AT+CNTP=\"time.google.com\",22"), ZF3E0, tout1));
  // GSM.GetCmd(F("AT+CNTP=\"pool.ntp.org\",22"), ZF3E0, tout1);

  // AT+CNTP // sync time
  // Serial.println(GSM.GetCmd(F("AT+CNTP"), ZF3E0, tout1));
  // GSM.GetCmd(F("AT+CNTP"), ZF3E0, tout1);

  // Serial.println(GSM.GetCmd(F("AT+CCLK?"), ZF3E0, tout1));
  // GSM.GetCmd(F("AT+CCLK?"), ZF3E0, tout1);

  // AT+HTTPPARA=\"CID\",1\r\n")
  GSM.CheckCmd(F("AT+HTTPPARA =\"CID\",1"), ZF3E0, tout1);

  //if check for https: -> turn on SSL (AT+HTTPSSL=1) ->
  GSM.Println(F("AT+HTTPSSL="));
  if (!CheckString(&Host, F("s://"), 4))
  {
    GSM.Println("0"); //not SSL
  }
  else
  {
    GSM.Println("1");
  }
  GSM.CheckCmd(F(""), ZF3E0, tout1);
  // Serial.println(GSM.GetCmd(F("AT+HTTPSSL=1"), ZF0Eo1, tout1));

  // +HTTPACTION: <code>
  // <code>  605 SSL failed to establish channels
  //         606 SSL alert message with a level of fatal result in the immediate termination of the connection

  // "AT+HTTPPARA=\"URL\",\""
  GSM.Print(F("AT+HTTPPARA=\"URL\",\""));

  // <Host> - S_HOST (with http or https)
  GSM.Print(&Host);
  GSM.Print(F("/"));

  // <Method> - S_METHOD
  if (server == SERVER1)
    GSM.Print(ReadeepString(S_METHOD1));
  else
    GSM.Print(ReadeepString(S_METHOD));

  // "\""
  GSM.Println(F("\""));
  GSM.CheckCmd(F(""), ZF3E0, tout1);

  //AT+HTTPPARA ="REDIR",1 -> redirect flag
  GSM.CheckCmd(F("AT+HTTPPARA =\"REDIR\",1"), ZF3E0, tout1);

  // "AT+HTTPPARA=\"CONTENT\",\""
  GSM.Print(F("AT+HTTPPARA=\"CONTENT\",\""));

  // <content> - S_CONTENT
  // GSM.Print(F("application/x-www-form-urlencoded"));
  if (server == SERVER1)
    GSM.Print(ReadeepString(S_CONTENT1));
  else
    GSM.Print(ReadeepString(S_CONTENT));

  // "\""
  GSM.Println(F("\""));
  GSM.CheckCmd(F(""), ZF3E0, tout1);

  return 1;
}

uint8_t Req_UPLOAD(uint16_t l)
{
  String st;
  st = F("AT+HTTPDATA=");
  st += (String)l;
  st += F(",5000"); //1000
  //  Serial.println(st);
  if (!GSM.CheckCmd(&st, F("DOW"), ZF3E0, toutHTTPDATA))
  {
    return 0;
  }
  return 1;
}

uint8_t HTTPstatus()
{
  //  AT+HTTPSTATUS?
  // +HTTPSTATUS: POST,1<0-idle, 1-Recv.., 2-Sending>,0<finish>,241<remaining to be sent or recv>
  //  Serial.println(F("STATUS"));
  // delay(20);//relaxation
  uint32_t t = millis();
  // wait 20ms
  while ((millis() - t) < 20)
    ; //test

  t = millis() / 100;
  while (GSM.Getuint8(F("AT+HTTPSTATUS?"), F(","), 1) != 0)
  {
    if (((millis() / 100) - t) > (toutSTATUS / 100))
      return 0;
  }
  return 1;
}

uint32_t Response()
{
  // SAFELY Check for +HTTPACTION: 1<POST>,200<OK>,3<LEN>
  // Serial.println(GSM.GetCmd(F(""), F("+HTTP"), ZF0E0, toutHTTP));

  // +HTTPACTION: <code>
  // <code>  605 SSL failed to establish channels
  //         606 SSL alert message with a level of fatal result in the immediate termination of the connection

  //  Serial.println(GSM.GetCmd(F(""), F("\r"), ZF0E0, tout1));
  String st;
  st = GSM.GetCmd(F(""), F("ON:"), ZF0Eo1, toutHTTP); //+HTTPACTION:
  // if(SHOW_SERIAL){
  //   Serial.print(F("Response()1: "));
  //   Serial.println(st);
  // }
  //+HTTPACTION:
  if (!CheckString(&st, F("+HT"), 3))
  { // && CheckString(&st, F(":"), 1)) {
    goto err;
  }
  st = GSM.GetCmd(F(""), F("\n"), ZF0Eo1, toutHTTP); //f2e01
  if (SHOW_SERIAL)
  {
    Serial.print(F("Response()2: "));
    Serial.println(st);
  }
  //1,200,5
  if (!CheckString(&st, F(","), 1))
  { //check two times ','
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

uint8_t Reply(uint16_t allowed_len)
{
  uint32_t r = Response(); //0-15-> reply status, 16-31->len
  if (SHOW_SERIAL)
  {
    Serial.print(F("RESPONSE: "));
    Serial.println(r, HEX);
  }
  // Return reply status ONLY IF LENTH IS <300
  // check reply status (MASK 0x00000FFC)
  //  r&0x00000FFC -> check series - 1xx, 2xx, 3xx, 4xx, 5xx, 6xx
  if (((r & 0x00000FFC) == 200))
  {
    // r&0x00000003
    //200, 201, 202
    if ((r & 0x00000003) > 2)
      return 0;
    // check suitable length
    r >>= 16;
    if ((r & 0x0000FFFF) > allowed_len)
    {
      return 0;
    }
  }
  else if (((r & 0x00000FFF) == 603))
  {
    ///IF DNS ERROR(603) close GPRS
    //AT+SAPBR=0,1 or AT+CIPSHUT
    Serial.println(F("DNS ERROR or No Internet"));
    GSM.CheckCmd(F("AT+SAPBR=0,1"), ZF3E0, tout1);
  }
  return 1;
}

uint8_t Uploded()
{
  //{"success":true}
  // return 1;
  // if (SHOW_SERIAL) {
  //     Serial.println(F("READ"));
  //     Serial.println(GSM.GetCmd(F("AT+HTTPREAD"), ZF3E0, tout1));
  // }
  if (GSM.CheckCmd(F("AT+HTTPREAD"), F("ACK"), ZF1E0, 1000))
  {
    return 1;
  }
  return 0;
}

void preTransmission()
{
  // digitalWrite(MAX485_RE, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  // digitalWrite(MAX485_RE, 0);
  digitalWrite(MAX485_DE, 0);
}

//////////////////////////////////////////////////////////////////
//FASTER AND TAKE SAME RAM AS POINTER advanced
uint8_t mod_write_para(uint8_t id, uint8_t slav, uint8_t func, uint16_t addr, uint8_t nreg)
{ //Advanced
  wdt_reset();
  Serial.println(freeRam());
  //  0xFF is terminator
  if (id == 0)
  {
    return 0;
  }
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
  {
    wdt_reset();
    add_ID = START_add_ID + ((eeprom - 1) * sizeof(modbus));
    if (EEPROM.read(add_ID) == 0xFF)
    {
      EEPROM.update(add_ID, 0);
    }
  }
  add_ID = START_add_ID + ((save.ID - 1) * sizeof(modbus));
  {
    eeprom = EEPROM.read(add_ID);
    if (eeprom == 0xFF)
    {
      goto write;
    } //not found
    else if (eeprom == save.ID)
    {
      goto overwrite;
    } //found
    else if (eeprom > save.ID)
    {
      goto recover;
    } //found
  }
  //  if found then overwrite
overwrite:
  Serial.print(id);
  Serial.print(F(" is over Written"));
  //  Serial.print(F(" at "));
  //  Serial.println(add_ID);
  goto exitt;
recover:
  add_ID -= sizeof(save);
  Serial.print(id);
  Serial.print(F(" is Recovered"));
  // Serial.print(F(" at "));
  // Serial.println(add_ID);
  goto exitt;
  //  else write at empty space
write:
  //NEW DATA WRITE NEED TO CHECK INDEXING THEN REINDEX
  Serial.print(id);
  Serial.print(F(" is written"));
  //  Serial.print(F(" at "));
  //  Serial.println(add_ID);
exitt:
  Serial.println(freeRam());
  EEPROM.put(add_ID, save); //save into memory
  Serial.println(freeRam());
  wdt_reset();
  return 1;
}

void frame()
{
  wdt_reset();
  Serial.print(F("|"));
  for (int i = 1; i <= 5; i++)
    Serial.print(F("------\t|"));
  Serial.println();
  Serial.println(F("|  SNo\t|  ID\t| FunC\t| Addr\t| POLLS\t|"));
  Serial.print(F("|"));
  for (int i = 1; i <= 5; i++)
  {
    Serial.print(F("------\t|"));
  }
  Serial.println();
}

uint8_t mod_read_para(uint8_t id, bool table)
{ //advanced
  wdt_reset();
  //  Serial.print(F("Reading data of ID-"));
  //  Serial.println(id);
  //same id should not avail twice
  uint8_t eeprom;
  // modbus mod;
  //  for (add_ID = START_add_ID; add_ID <= last_ID; add_ID += sizeof(modbus))
  add_ID = START_add_ID + ((id - 1) * sizeof(modbus));
  eeprom = EEPROM.read(add_ID);
  if (eeprom == 0x00)
  {
    goto not_found;
  } //not found
  else if (eeprom == 0xFF)
  {
    goto end_db;
  } //db end
  else if (eeprom == id)
  {
    //done:
    //Write data to variable 'o'
    //  Serial.println(add_ID);
    EEPROM.get(add_ID, mod);
    // Serial.print(F("ID-"));
    // Serial.println(id);
    if (table)
    {
      Serial.print(F("ID-"));
      Serial.println(id);
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
      wdt_reset();
      for (int i = 1; i <= 5; i++)
      {
        Serial.print(F("------\t|"));
      }
      Serial.println();
      Serial.println(F("Reading Done"));
    }
    wdt_reset();
    return 1;
  }
not_found:
  Serial.println(F("Already deleted"));
  return 0;
end_db:
  Serial.println(F("Db end"));
  return 2;
}

// 0xFF -> END
//0x00 -> Data Delete
void mod_show_para()
{ //advance
  wdt_reset();
  modbus read;
  uint8_t no_data = 0; //let we have no data
  Serial.println(F("Reading Table:"));
  frame();
  for (add_ID = START_add_ID; add_ID <= last_ID; add_ID += sizeof(modbus))
  {
    wdt_reset();
    EEPROM.get(add_ID, read); //read from mrmory
    if (read.ID == 0xFF)
    {
      goto exitt;
    } //0xFF db end
    if (read.ID == 0)
    {
      no_data++;
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
  {
    Serial.print(F("------\t|"));
  }
  Serial.println();
  wdt_reset();
  add_ID -= sizeof(modbus);
  //  Serial.println(add_ID);
  //  Serial.println(last_ID);
  //  Serial.println(no_data);
  if (no_data == (((add_ID - START_add_ID) / sizeof(modbus)) + 1))
  {
    Serial.println(F("No data"));
    while (no_data > 0)
    {
      // Serial.print(F("Setting "));
      // Serial.print(add_ID);
      // Serial.println(F(" to 0xFF"));
      EEPROM.update(add_ID, 0xFF);
      add_ID -= sizeof(modbus);
      no_data--;
      wdt_reset();
    }
  }
  wdt_reset();
}

void mod_del_para(uint8_t i, uint8_t f)
{ //advanced
  if (i == f)
  {
    Serial.println(F("Single DELETE"));
  }
  if (f < i)
  {
    Serial.println(F("Invalid Entery"));
    goto exitt;
  } //seek to address and set id to zero
  add_ID = START_add_ID + ((i - 1) * sizeof(modbus));
  for (; i <= f; i++)
  {
    wdt_reset();
    if (EEPROM.read(add_ID) == 0xFF)
    {
      goto exitt;
    }
    EEPROM.update(add_ID, 0);
    //delete old dynamic data block
    SafeDel(&d_mod[i - 1].d);
    //    Serial.print(i);
    //    Serial.print(F(" is deleted at "));
    //    Serial.println(add_ID);
    add_ID += sizeof(modbus);
  }
exitt:
  Serial.println(F("Done deleting"));
}

//////////////////////////////////////////////////////////////

void beginRS485(void)
{
  // RS485.setRx(PB7);//RO
  // RS485.setTx(PB6);//DI
  // uint8_t mod_parity_var = get_mod_parity();//eeprom1
  //RS485
  if (mod_parity_var == 1)
  {
    RS485.begin(mod_baud_100 * 100, SERIAL_8N1);
  }
  else if (mod_parity_var == 2)
  {
    RS485.begin(mod_baud_100 * 100, SERIAL_8O1);
  } //TAKE USER INPUT
  else if (mod_parity_var == 3)
  {
    RS485.begin(mod_baud_100 * 100, SERIAL_8E1);
  } //TAKE USER INPUT

  pinMode(MAX485_DE, OUTPUT);
  // node.begin(1, RS485);

  //VVI Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void beginGSM()
{
  GSM_SERIAL.setRx(PA10); //RO
  GSM_SERIAL.setTx(PA9);  //DI
  // RESTORE GPRS BAUD
  GSM_SERIAL.end();
  GSM_SERIAL.begin(gsmbaud, SERIAL_8N1); //GSM
}

void FreeHeap(void)
{
  for (uint8_t i = 0; i < nBlock; i++)
  {
    if (d_mod[i].d)
    {
      SafeDel(&d_mod[i].d);
      // Serial.print(i);
      // Serial.println(F(" is Deleted"));
    }
    // else {
    //     Serial.print(i);
    //     Serial.println(F(" is Free"));
    // }
  }
}

bool CheckID(uint8_t slave)
{
  return (slave == 0 || slave > 247) ? false : true;
}

uint16_t read_data_DB()
{
  uint8_t j, result = 255; //j- no. of attempts
  uint16_t len = 0;
  data_count = 0;
  // modbus mod;
  FreeHeap();

  beginRS485(); //CONFIGURE RX TX PIN
  for (uint8_t i = 0; i < nBlock; i++)
  {
    //get and update j with mod_retry_var
    // digitalWrite(MODBUS, 0);
    j = mod_retry_var - 1;   //eeprom2
    digitalWrite(MODBUS, 0); //Y-S
    Serial.println(freeRam());
    if (mod_read_para(i + 1, 0) != 1)
    { //reading parameter from ram
      if (mod_read_para(i + 1, 0) == 0)
      {
        d_mod[i].result = 0;
        continue;
      }
      else if (mod_read_para(i + 1, 0) == 2)
      { //ALL read
        d_mod[i].result = -1;
        goto out;
      }
    }
    else
    { //delete old
      // if(d_mod[i].d){SafeDel(&d_mod[i].d);
      //     Serial.print(i);
      //     Serial.println(F(" is deleted"));
      // }
      // else {
      //     Serial.print(i);
      //     Serial.println(F(" is free"));
      // }

      //check slave id
      if (!CheckID(mod.slave))
      {
        d_mod[i].result = 0;
        continue;
      }
      // retry:
      //ram para read success
      node.begin(mod.slave, RS485);
      // Callbacks allow us to configure the RS485 transceiver correctly
      node.preTransmission(preTransmission);
      node.postTransmission(postTransmission);
      delay(15); //breath for another read
      Serial.print(F("SNo.: "));
      Serial.print(i + 1);
      Serial.print(F("\t\tSLAVE ID: "));
      Serial.println(mod.slave);

    retry:
      //function code 3
      wdt_reset();
      if (mod.fun == 3)
      {
        result = node.readHoldingRegisters(mod.address, mod.registers);
      }
      //function code 4
      else if (mod.fun == 4)
      {
        result = node.readInputRegisters(mod.address, mod.registers);
      }
      if (result != 0x00)
      {
        //IF RESULT!=0 ATTEMPT N-TIMES AS PER USER
        fail(MODBUS);
        if (result == 0x02)
        {
          Serial.println(F("MODBUS: Illigal Data Add."));
        }
        else if (result == 0x03)
        {
          Serial.println(F("MODBUS: Illigal Data Value"));
        }
        else if (result == 0x04)
        {
          Serial.println(F("MODBUS: Slave Failure"));
        }
        else if (result == 0xE1)
        {
          Serial.println(F("MODBUS: Invalid Function"));
        }
        else if (result == 0xE2)
        {
          Serial.println(F("MODBUS: Timeout"));
        } //RETRY
        else if (result == 0xE0)
        {
          Serial.println(F("MODBUS: Invalid Slave id"));
        } //RETRY
        else if (result == 0xE3)
        {
          Serial.println(F("MODBUS: Invalid CRC"));
        }
        if (j != 0)
        { //since starting with zero
          j--;
          goto retry;
        }
        else
        {
          //continue to for loop again
          d_mod[i].result = 0;
          continue;
        }
      }
      {                          //mod success
        digitalWrite(MODBUS, 1); //RED
        if (EEPROM.read(0) > 0x00)
        {
          Serial.print(F("MODBUS: "));
          Serial.print(mod.ID);
          Serial.println(F(" OK"));
        }
        //calloc and reserve memory for string
        d_mod[i].result = mod.registers;
        len += (2 * mod.registers);
        data_count++;
        d_mod[i].d = (uint16_t *)calloc(mod.registers, sizeof(uint16_t)); //uint16_t data

        //for 16-bit
        for (j = 0; j < mod.registers; j += 2)
        {
          *(d_mod[i].d + j) = node.getResponseBuffer(j);
          *(d_mod[i].d + (j + 1)) = node.getResponseBuffer(j + 1);
          wdt_reset();
          //  { Serial.print(mod.address + (j + 1));
          //    Serial.print(F("="));
          //    u.d = *(d_mod[i].d + (j + 0));
          //    u.d = u.d << 16;
          //    u.d = u.d | *(d_mod[i].d + (j + 1));
          //         //       u.d = d32;
          //         // Serial.println(data[j], HEX);
          //         // Serial.print(u.d, HEX);
          //    Serial.print(u.d, HEX);
          //    Serial.print(F("="));
          //    Serial.println(u.f);
          //  }
        }
        // print_result:
        // if(EEPROM.read(0) > 0x01){
        //   for (j = 0; j < d_mod[i].result; j++) {
        //     Serial.print(*(d_mod[i].d + j), HEX);
        //   }
        // Serial.println();
        // }
        //            safe_del(&d_mod[i].d);
      }
    }
  } //END FOR
// return 8-bit length
out:
  digitalWrite(MODBUS, 0); //RED
  return len;
} //SAVES RAM 4B

void put_data(uint8_t type)
{

  //ID=   -> 3 + 8
  if (type == TYPE_ABP)
  {
    DGSM.print(F("ID="));
  }
  else
  {
    DGSM.print(F("DEV="));
  }
  DGSM.print(h2s(client_id_var, 16));
  DGSM.print(h2s(device_id_var, 16));
  //&DT=  -> 4 + 8
  DGSM.print(F("&DT="));
  DGSM.print(h2s((uint32_t)0, 32));
  //&G=   -> 3
  if (type == TYPE_ABP)
  {
    DGSM.print(F("&G="));
  }
  else
  {
    DGSM.print(F("&DATA="));
  }

  for (uint8_t i = 0; i < nBlock; i++)
  {
    if (!d_mod[i].d)
    {
      continue;
    }
    mod_read_para(i + 1, 0);
    //START ADD -> 16-bit
    DGSM.print(h2s((uint16_t)mod.address, 16));
    //SLAVE ID  -> 8-bit
    DGSM.print(h2s((uint16_t)mod.slave, 8));
    //MOD FUN   -> 8-bit
    DGSM.print(h2s((uint16_t)mod.fun, 8));
    //nBytes    -> 8-bit
    DGSM.print(h2s((uint16_t)(2 * mod.registers), 8));

    for (uint8_t j = 0; j < d_mod[i].result; j++)
    {
      //16-bit padding
      // Serial.print(h2s(String(*(d_mod[i].d + j), HEX), 16));
      DGSM.print(h2s(*(d_mod[i].d + j), 16));
      // length += 2;
    }
    if (type == TYPE_ABP)
    {
      DGSM.print(F("/")); //len++
    }
    // length++;
  }
}

void Serial_put_data(uint8_t type)
{

  //ID=   -> 3 + 8
  if (type == TYPE_ABP)
  {
    Serial.print(F("ID="));
  }
  else
  {
    Serial.print(F("DEV="));
  }
  Serial.print(h2s(client_id_var, 16));
  Serial.print(h2s(device_id_var, 16));
  Serial.print(F("(Client id:- "));
  Serial.print(client_id_var);
  Serial.print(F("\tdevice id:- "));
  Serial.print(device_id_var);
  Serial.println(F(")"));
  //&DT=  -> 4 + 8
  Serial.print(F("DT="));
  Serial.println(h2s((uint32_t)0, 32));
  //&G=   -> 3
  if (type == TYPE_ABP)
  {
    Serial.print(F("G="));
  }
  else
  {
    Serial.print(F("DATA="));
  }

  for (uint8_t i = 0; i < nBlock; i++)
  {
    if (!d_mod[i].d)
    {
      continue;
    }
    mod_read_para(i + 1, 0);
    //START ADD -> 16-bit
    Serial.print(h2s((uint16_t)mod.address, 16));
    //SLAVE ID  -> 8-bit
    Serial.print(h2s((uint16_t)mod.slave, 8));
    //MOD FUN   -> 8-bit
    Serial.print(h2s((uint16_t)mod.fun, 8));
    //nBytes    -> 8-bit
    Serial.print(h2s((uint16_t)(2 * mod.registers), 8));

    for (uint8_t j = 0; j < d_mod[i].result; j++)
    {
      //16-bit padding
      // Serial.print(h2s(String(*(d_mod[i].d + j), HEX), 16));
      Serial.print(h2s(*(d_mod[i].d + j), 16));
      // length += 2;
    }
    if (type == TYPE_ABP)
    {
      Serial.print(F("/")); //len++
    }
    // length++;
  }
}

// static void data_frame(uint16_t len){
//   //datalen & number of data
//   uint16_t length = 0;
//   //3 + 4 + 3 +
//   //number of valid block
//   //calculate and check length of string
//   //ID=   -> 3 + 8
//   //&DT=  -> 4 + 8
//   //&G=   -> 3
//   //DATA_LEN + data_count
//   for(uint8_t i = 0; i < nBlock; i++) {
//     if(!d_mod[i].d) { continue;}
//     for (uint8_t j = 0; j < d_mod[i].result; j++) {
//       //16-bit padding
//         // Serial.print(h2s(String(*(d_mod[i].d + j), HEX), 16));
//         DGSM.print(h2s(*(d_mod[i].d + j), 16));
//         length += 2;
//     }
//     DGSM.print(F("/"));//len++
//     length++;
//   }
//   Serial.println();
//   Serial.println(length);
//   Serial.println(len + data_count);
// }

uint8_t UPLOAD_ENC_DATA(uint8_t server,  uint8_t type, uint16_t len)
{
  int8_t Retry = u_retry_var; //from Eeeprom//eeprom3
  // uint32_t wt;

beg:
  // wt = millis();
  GSM.Heal(GSMHeal | 0x80, RST); //stuck there
  Serial.println(F("U"));
  // GSM_Heal();
  //  //Serial.println(F("U..."));
  // Serial.println(GSM.GetSignal(SigPct));
  //CHECK GPRS
  wdt_reset();
  upload_1();
  //CPNNECT INTERNET
  if (!GSM.Internet(0b01000000))
  { //0b01000000
    if (SHOW_SERIAL)
    {
      Serial.println(F("!g"));
    }
    if (GSM.Test(NWtest))
    {
      notGPRS--;
    }
    //CHECK SIM OR HEAL GSM
    goto retry;
  }
  //sync time

  if (SHOW_SERIAL)
  {
    Serial.println(F("G.."));
  }
  //  POST:
  //  SIZE- 850 OK
  notGPRS--;
  if (!HTTPInit())
  {
    goto retry;
  }

  HTTPsetup(server);

  LED_BLINK->setOverflow(80000UL, MICROSEC_FORMAT); //100ms
  LED_BLINK->attachInterrupt(UPLOAD_BLINK);

  //calculate byte length of data
  //3 + 4 + 3 +
  //number of valid block
  //calculate and check length of string
  //ABP
  //ID=   -> 3 + 8
  //&DT=  -> 4 + 8
  //&G=   -> 3
  // =26
  //QINN
  //DEV=   -> 4 + 8
  //&DT=  -> 4 + 8
  //&DATA=   -> 6
  // =30

  //    START ADD -> 16-bit
  //    SLAVE ID  -> 8-bit
  //    MOD FUN   -> 8-bit
  //    nBytes    -> 8-bit
  //    /         -> 8-bit
  // TYPE_ABP
  //DATA_LEN + (data_count * 11)
  // TYPE_QINN
  //DATA_LEN + (data_count * 10)
  //Total len of frame = (2*dlen) + (LEN_DELE)
  Serial.println(F("Req:: Upload"));

  if (!Req_UPLOAD((type == TYPE_QINN ? 30 : 26) + (2 * len) + (data_count * (type == TYPE_QINN ? 10 : 11))))
  { //DOWNLOAD
    goto retry;
  }
  // if (!Req_UPLOAD((26) + (2 * len) + (data_count * (11))))
  // { //DOWNLOAD
  //   goto retry;
  // }
  Serial.print(F("Putting::"));
  Serial.println((type == TYPE_QINN ? 30 : 26) + (2 * len) + (data_count * (type == TYPE_QINN ? 10 : 11)));
  Serial_put_data(type);
  Serial.println();
  // Serial.println(millis() - wt);
  // put_data(TYPE_ABP);
  put_data(type);
  //don't put here
  // Serial.println(millis() - wt);
  //    check for:-
  //    OK
  if (!GSM.CheckCmd(F(""), ZF3E0, tout1))
  {
    goto retry;
  }

  // Serial.println(F("put ok"));
  // Serial.println(millis() - wt);
  notGPRS++;
  //  AT+HTTPACTION=1
  wdt_reset();
  // Serial.println(F("ACT=1"));
  Serial.println(F("UPLOADING...."));
  if (!GSM.CheckCmd(F("AT+HTTPACTION=1"), ZF3E0, tout1))
  { //POST
    goto retry;
  }
  //  AT+HTTPSTATUS?
  //  //Serial.println(F("STATUS"));
  //  //Serial.println(GSM.GetCmd(F("AT+HTTPSTATUS?"), ZF3E0, tout1));

  //check STATUS
  if (!HTTPstatus())
  {
    goto retry;
  }

  //////////// VERY SENSITIVE PART /////////////
  /*
        Accepted:- 200, 202,
        Rejected:-
        ERROR:-
    */

  if ((state & 0x01) == 0)
  {
    LED_BLINK->setOverflow(25000UL, MICROSEC_FORMAT);
    LED_BLINK->attachInterrupt(UPLOAD_BLINK_WAIT_on);
  }
  if ((state & 0x01) == 0x01)
  {
    LED_BLINK->setOverflow(200000UL, MICROSEC_FORMAT);
    LED_BLINK->attachInterrupt(UPLOAD_BLINK_WAIT_off);
  }

  if (!Reply(5000))
  {
    Serial.println(F("Server is sending out of limit reply"));
    goto retry;
  }

  // Serial.println(millis() -wt);

  //read data if Reply() is ok
  if (!Uploded())
  {
    Serial.println(F("ACK ERROR"));
    goto retry;
  }
  //ACCOTDING TO STATUS READ HTTP MSG

  Serial.println(F("UPLOADED"));
  //  Serial.write(0x0E);
  //  Serial.println();
  LED_BLINK->detachInterrupt();
  upload_1();
  wdt_reset();

  // ok:
  if (SHOW_SERIAL)
  {
    freeRam();
  }
  return 1;

retry:
  //  fail(UPLOAD);
  if (Retry > 0)
  {
    //CHECK BIT.11 THEN RETRY -= 2
    //    if (!((Response >> 11) & 0x01))//0x800
    // if (!(Response & 0x0800)) { //0x800
    //   Retry -= 2;
    //   // fail(UPLOAD);
    // }
    // else
    Retry--;
    //Serial.println(Retry);
    // //Serial.print(F("0x"));
    // //Serial.println(Response, HEX);
    // Response = 0;//reset response
    Serial.print(F("GPRS Fail count:: "));
    Serial.println(notGPRS);
    if (notGPRS <= 0)
    {
      // RESET MODEM
      if (GSM.Restart(RST) != 0)
      { //every 100ms//MEMORY FRAGMENTING
        GSM.Set();
        // GSM_Heal();
      }
      // HEAL MODEM
      notGPRS = NOT_INTERNET_COUNT;
      //Serial.println(F("EXTREME INTERNET ERROR"));
    }

    goto beg;
  }

  Serial.println(F("ERROR: UPLOAD FAILED"));
  // fail:
  fail(UPLOAD); //LED OFF
  digitalWrite(UPLOAD, OFF);
  //tun off internet
  // GSM.CheckCmd(F("AT+SAPBR=0,1"), ZF3E0, tout1);
  //RESTART REQUEST
  return 0;
}

void Eeeprom_UPDATE()
{
  String st;

  for (uint8_t ID = 1; ID <= nBlock; ID++)
  {
    wdt_reset();
    mod_write_para(ID, 1, 3, 0, 26);
  }
  // Optional_qDEV
  // WriteeepString(S_Optional_qDEV, E_Optional_qDEV, F("PrismL1"));
  // MC: // MODBUS COMMUNICATION
  // //get
  // EEPROM.update(MOD_FUN, GetuInt8(data, 2)); //MOD_FUNCTION
  // //get
  // EEPROM.update(PARITY, GetuInt8(data, 3)); //MOD_PARITY
  // //get
  // EEPROM.update(MOD_BAUD, GetuInt8(data, 4));//MOD_BAUD

  // MP: //MODBUS PARAMETER
  //  //######################## Slave id-1   //Fixed
  // EEPROM.update(SLAVE1, GetuInt8(data, &from, 1)); //SID-1
  // Write16Int(MOD_ADD1, GetuInt(data, &from, 2)); //ADD-1
  // EEPROM.update(nPOLL1, GetuInt8(data, &from, 3)); //REG-1
  // //CAUSE 164 BYTE OF FLASH
  // //Slave id-2
  // c[0] = GetuInt8(data, &from, 4);
  // EEPROM.update(SLAVE2, (uint8_t)c[0]); //SID-2
  // if ((uint8_t)c[0] != 0) {
  //   Write16Int(MOD_ADD2, GetuInt(data, &from, 5)); //ADD-2
  //   EEPROM.update(nPOLL2, GetuInt8(data, &from, 6)); //REG-2
  // }
  // //SO ON

  // CR: //WiFi Connect Router
  // save:
  // //SAVE TO EEPROM-
  // WriteeepString(S_SSID, E_SSID, &ssid);  //WRITE SSID
  // WriteeepString(S_PASS, E_PASS, &password);  //WRITE KEY

  // SA: set access point
  // http://bhanu.free.beeceptor.com/G_POST
  // WriteeepString(S_HOST, E_HOST, F("http://demo5089036.mockable.io")); //Host
  // WriteeepString(S_METHOD, E_METHOD, F("/G_SSLTEST")); //Method
  //  https://epaqtbbwt7.execute-api.ap-south-1.amazonaws.com/dev
  // WriteeepString(S_HOST, E_HOST, F("https://epaqtbbwt7.execute-api.ap-south-1.amazonaws.com")); //Host
  // WriteeepString(S_METHOD, E_METHOD, F("/dev")); //Method

  // GSM.CheckCmd(F("AT+HTTPPARA=\"URL\",\"http://34.196.106.201/postdata\""), ZF3E0, tout1);
  //  AT+HTTPPARA="URL","http://103.233.79.197:8000/ADSmart.asmx/Demo1
  // GSM.CheckCmd(F("AT+HTTPPARA=\"URL\",\"https://www.techiebros.in/metricsbro/index.php/api/metrics/raw_logs\""), ZF3E0, tout1);
  // GSM.CheckCmd(F("AT+HTTPPARA=\"URL\",\"http://13.127.159.141/metricsbro/log.php\""), ZF3E0, tout1);
  // GSM.CheckCmd(F("AT+HTTPPARA=\"URL\",\"http://demo5089036.mockable.io/G_TEST\""), ZF3E0, tout1);

  //  https://wsms.co.in
  //  /api/v1/[IMEI_NUMBER]/telemetry
  //  AT+GSN -> GET IMEI(FAIL)  867273021268029
  // WriteeepString(S_HOST, E_HOST, F("http://34.196.106.201")); //Host
  // st = F("/api/v1/");https://wsms.co.in/api/v1/867273021268029/telemetry
  // WriteeepString(S_METHOD, E_METHOD, F("/api/v1/867273021268029/telemetry")); //Method//site-2
  //  WriteeepString(S_METHOD, E_METHOD, F("/api/v1/865472039224295/telemetry")); //Method
  WriteeepString(S_HOST, E_HOST, F("http://api.smartfi.tech")); //Host
  WriteeepString(S_METHOD, E_METHOD, F("/postdata"));           //Method//WILL BE IN site-1

  // http://86.96.203.99:81/api/parsermbus
  // WriteeepString(S_HOST, E_HOST, F("http://86.96.203.99:81")); //Host
  // WriteeepString(S_METHOD, E_METHOD, F("/api/parsermbus")); //Method

  // WriteeepString(S_HOST, E_HOST, F("http://bhanu.free.beeceptor.com")); //Host
  // WriteeepString(S_METHOD, E_METHOD, F("/G_POST")); //Method

  // application/x-www-form-urlencoded
  WriteeepString(S_CONTENT, E_CONTENT, F("application/x-www-form-urlencoded")); //Content Type

  //text/json
  // WriteeepString(S_CONTENT, E_CONTENT, F("text/json"));

  // SI: //SET ID
  //SAVE TO EEPROM
  Write16Int(CLIENT_ID, 10);
  Write16Int(DEVICE_ID, 2);

  // DU: //data upload time and retry
  EEPROM.update(UPLOAD_d, (50 / 5)); //UPLOAD
  //upload retry
  EEPROM.update(U_RETRY, 1);

  // DR: //DATA RETRY
  EEPROM.update(MOD_RETRY, 3); //mod_retry

  //SSL: ON or OFF

  // DT: // DEVICE TEST
}

void Eeeprom_SHOW()
{
  // Serial.println(ReadeepString(S_Optional_qDEV));
  //460 BYTE PROM
  //EEPROM TEST
  // Serial.println(EEPROM.read(SLAVE1));
  // Serial.println(EEPROM.read(SLAVE2));
  // Serial.println(EEPROM.read(SLAVE3));
  // Serial.println(EEPROM.read(SLAVE4));
  Serial.println(get_mod_baud() * 100);
  Serial.println(get_mod_parity());
  // Serial.println(get_mod_fun());
  Serial.println(get_mod_retry());
  //4*Slave, Add, nPoll

  //Upload Duration
  Serial.println(get_upload() * 5);
  Serial.println(get_client_id());
  Serial.println(get_device_id());

  //  / / String
  //   Serial.println(ReadeepStringUpto(S_SSID, '\0'));
  //   Serial.println(ReadeepStringUpto(S_PASS, '\0'));
  Serial.println(ReadeepString(S_HOST));
  //   Serial.println(ReadeepStringUpto(S_HOST, ':'));
  Serial.println(ReadeepString(S_METHOD));
  Serial.println(ReadeepString(S_CONTENT));
}

void GSM_Heal()
{
  GSM.Heal(RST); //Restrt rst pin 0-sw rst !0-hw reset // (-2)->SIM Not Ready (-1)-> Call Ready not found (0)-> !AT (1)-> GSM DEVICE READY
  if (EEPROM.read(0) == 0xFF)
  {
    GSM.Set(ONE_TIME_BASIC);
    EEPROM.write(0, 0xFE);
  }
  // GSM.Set();//MULTIPLE TIME SET//ALREADY DONE GSM.Heal
  // if(SHOW_SERIAL){Serial.print(F("N/w "));}
}

void app(char *command, String *data)
{ //app command executed here
  //  Serial.print(*command);
  //  Serial.println(*(command + 1));
  //  Serial.println(*data);

  char c[5] = {'C', 'D', 'M', 'S', 'R'};
  //detect commad
  if (*command == c[0] || *command == c[1] || *command == c[2] || *command == c[3])
  {
    //send reply GetIt and check for liveid
    if (Serial.println(F("<title>GetIt")))
    {
      if (*command == c[1])
      {
        if (*(command + 1) == 'U')
          goto DU; //SET UPLOAD PERIOD
        else if (*(command + 1) == c[4])
          goto DR; //MOD DATA RETRY
        else if (*(command + 1) == 'L')
          goto DL; //DELETE TABLE
      }
      else if (*command == c[2])
      {
        if (*(command + 1) == c[0])
          goto MC; //SET MODBUS COMM
        if (*(command + 1) == 'P')
          goto MP; //SET MODBUS PARAMETERS
      }
      else if (*command == c[3])
      {
        if (*(command + 1) == 'A')
          goto SA; //SET IP PORT
        else if (*(command + 1) == 'I')
          goto SI; //SET ID
        else if (*(command + 1) == 'P')
          goto SP; //SHOW PARA
      }
    } //replt get it
    else
      goto err;
  }
  else
  {
    //    WIFI.Reply(id, &ok);
    goto ok;
  }
MC:
{ //set modbus comuunication- "6?3?2? " =
  Serial.println(F("SETTING: Modbus Commmunication"));
  //old //set modbus communication- "8?1?3?2?3? "-"<len>?<slave_id>?<mod_fun>?<parity>?<4800*(2^(BAUD-1))>
  //use *command as '?'
  //    *command = '?';
  if (CountOf(data, '?') != 3)
  {
    Serial.println(F("ERROR: Setting Modbus Commmunication"));
    goto err;
  }
  //get
  // EEPROM.update(MOD_FUN, GetuInt8(data, 2)); //MOD_FUNCTION
  // // Serial.println(GetuInt8(data, 2)); //MOD_FUNCTION
  //get
  EEPROM.update(PARITY, GetuInt8(data, 2)); //MOD_PARITY
  // Serial.println(GetuInt8(data, 3)); //MOD_PARITY
  //get
  EEPROM.update(MOD_BAUD, GetuInt8(data, 3)); //MOD_BAUD
  // Serial.println(GetuInt8(data, 4)); //MOD_BAUD
  mod_parity_var = get_mod_parity();
  mod_baud_100 = get_mod_baud();
  //get
  //begin
  Serial.print(F("NEW: Modbus Parity-->"));
  if (mod_parity_var == 1)
    Serial.println(F("NONE"));
  else if (mod_parity_var == 2)
    Serial.println(F("ODD"));
  else if (mod_parity_var == 3)
    Serial.println(F("EVEN"));
  Serial.print(F("NEW: Modbus Baud-->\t"));
  Serial.println(mod_baud_100 * 100);
  // begin_rs485();
  // ESP.listen();
  goto ok;
}
MP:
{ //21?1?3839?2?0???0???0???
  Serial.println(F("SETTING: Modbus Parameter"));
  //set modbus parameter- 20?1?1?3?3839?62?2?2?3?0?13? -<len>?<ID>?<SLAVE>?<FUN>?<S_ADD>?<REG>?<ID>?<SLAVE>?<FUN>?<S_ADD>?<REG>?
  //use *command as '?'
  //    *command = '?';
  //VALIDATE DATA -> count numbe of '?' -> (n-1)%5 should be zero
  int16_t count = (CountOf(data, '?') - 1);
  if ((count % nMP) != 0)
  {
    goto err;
  }
  // String from = F("?");
  //######################## Slave id-1   //Fixed
  //1 -> ID
  //2 -> SLAVE
  //3 -> FUN
  //4 -> ADDRESS
  //5 -> REG
  // Serial.println(count);
  // mod_del_para(1, nBlock);
  while (count > 0)
  {
    // Serial.println(GetuInt8(data, F("?"), (count - nMP1)));
    // Serial.println(GetuInt8(data, F("?"), (count - nMP2)));
    // Serial.println(GetuInt8(data, F("?"), (count - nMP3)));
    // Serial.println(GetuInt(data, F("?"), (count - nMP4)));
    // Serial.println(GetuInt8(data, F("?"), count));
    mod_write_para(GetuInt8(data, F("?"), count - nMP1) /*ID*/, GetuInt8(data, F("?"), count - nMP2) /*SLAVE*/, GetuInt8(data, F("?"), count - nMP3) /*FUN*/, GetuInt(data, F("?"), count - nMP4) /*ADDRESS*/, GetuInt8(data, F("?"), count) /*REG*/);
    count -= nMP;
  }
  Serial.println(F("NEW: Modbus parameter successfully set"));
  mod_show_para();
  goto ok;
}
SA:
{ //Set Host, Method and port- 20?http://google.com?mail?text/plain?  = <len>?<host>?<method>?<content>
  Serial.println(F("SETTING: ACCESS POINT"));
  //old //set ip and port-19?172?233?79?23?8000? = <len>?<ip1>?<ip2?<ip3>?<ip4>?<port>
  //use *command as '?'
  //    command = '?';
  if (CountOf(data, '?') != 4)
  {
    goto err;
  }
  //USE AS HOST SAVING
  String from = F("?");
  c[0] = '?';
  //    Serial.println(*data);
  WriteeepString(S_HOST, E_HOST, TrimString(data, &from, &c[0], 1));     //Host
  WriteeepString(S_METHOD, E_METHOD, TrimString(data, &from, &c[0], 2)); //Method
  //"text/plain"
  WriteeepString(S_CONTENT, E_CONTENT, TrimString(data, &from, &c[0], 3)); //Content-Type:
  //    Serial.println(ReadeepString(S_CONTENT));
  Serial.print(F("NEW: Data Server-->\t"));
  Serial.print(ReadeepString(S_HOST));
  Serial.print(F("/"));
  Serial.println(ReadeepString(S_METHOD));
  Serial.print(F("NEW: Content Type-->\t"));
  Serial.println(ReadeepString(S_CONTENT));
  goto ok;
}
SI:
{ //client id device id -4?1?2?
  Serial.println(F("SETTING: CLIENT & DEVICE ID"));
  //use *command as '?'
  //    command = '?';
  if (CountOf(data, '?') != 3)
  {
    goto err;
  }
  //SAVE TO EEPROM
  Write16Int(CLIENT_ID, GetuInt(data, 2));
  Write16Int(DEVICE_ID, GetuInt(data, 3));
  client_id_var = get_client_id();
  device_id_var = get_device_id();
  //    reply OK then
  Serial.print(F("NEW: Client ID-->\t"));
  Serial.println(client_id_var);
  Serial.print(F("NEW: Device ID-->\t"));
  Serial.println(device_id_var);
  goto ok;
}
SP:
{ //show para
  Serial.println(F("SHOW: SETTINGS"));
  Serial.println();
  Serial.print(F("Modbus Baud-->\t"));
  Serial.println(mod_baud_100 * 100);
  Serial.print(F("Modbus Parity-->"));
  if (mod_parity_var == 1)
    Serial.println(F("NONE"));
  else if (mod_parity_var == 2)
    Serial.println(F("ODD"));
  else if (mod_parity_var == 3)
    Serial.println(F("EVEN"));
  Serial.print(F("Modbus Retry-->\t"));
  Serial.println(mod_retry_var);
  Serial.print(F("Upload Retry-->\t"));
  Serial.println(u_retry_var);
  Serial.print(F("U(s)-->\t\t"));
  Serial.println(get_upload_var * 5);
  Serial.print(F("Client ID-->\t"));
  Serial.println(client_id_var);
  Serial.print(F("Device ID-->\t"));
  Serial.println(device_id_var);
  Serial.print(F("Data Server-->\t"));
  Serial.print(ReadeepString(S_HOST));
  Serial.print(F("/"));
  Serial.println(ReadeepString(S_METHOD));
  Serial.print(F("Content Type-->\t"));
  Serial.println(ReadeepString(S_CONTENT));
  mod_show_para();
  goto ok;
}
DU:
{ //upload duration
  //use *command as '?'
  //    command = '?';
  Serial.println(F("SETTING: UPLOAD Duration & Retry"));
  if (CountOf(data, '?') != 3)
  {
    goto err;
  }
  EEPROM.update(UPLOAD_d, (GetuInt8(data, 2))); //UPLOAD
  //upload retry
  EEPROM.update(U_RETRY, GetuInt8(data, 3));
  get_upload_var = get_upload();
  u_retry_var = get_u_retry();

  Serial.print(F("NEW: U(s)-->\t\t"));
  Serial.println(get_upload_var * 5);
  Serial.print(F("NEW: Upload Retry-->\t"));
  Serial.println(u_retry_var);
  //get
  goto ok;
}
DR:
{ //modbud retry
  //use *command as '?'
  //    command = '?';
  Serial.println(F("SETTING: Modbus Data Retry"));
  if (CountOf(data, '?') != 2)
  {
    goto err;
  }
  EEPROM.update(MOD_RETRY, GetuInt8(data, 2)); //mod_retry
  //get
  mod_retry_var = get_mod_retry();
  Serial.print(F("NEW: Modbus Retry-->\t"));
  Serial.println(mod_retry_var);
  goto ok;
}
DL:
{
  Serial.println(F("SETTING: Data Delete"));
  int16_t count = (CountOf(data, '?') - 1);
  uint8_t id;
  while (count > 0)
  {
    id = GetuInt8(data, F("?"), count--);
    if (id == 0)
    {
      Serial.println(F("Delete: ALL Modbus Parameters"));
      mod_del_para(1, nBlock);
      FreeHeap();
    }
    else
    {
      Serial.println(F("Delete: Following Modbus Parameters"));
      mod_read_para(id, 1);
      mod_del_para(id, id);
    }
  }
  goto ok;
}
// DT: { //test connection with server
//     //use *command as '?'
//     //    command = '?';
//     //    c[0] = ',';//add with *data
//     uint16_t result = upload_enc_buff(0); //MSB-result show error status//LSB-SHOWS SERVER ERROR
//     //decode data
//     //    result&0x1FF//server reply
//     *data = h2s((result & 0x1FF), 12);
//     //        *data = (result & 0x1FF);
//     //result >> 9 & 0x01//AT ERROR          //MODULE ERROR
//     //result >> 10 & 0x01//JAP ERROR        //JAP ERROR//NOT USED
//     //result >> 11 & 0x01//CONNECT ERROR    //JAP OR Internet OR HOST ERROR
//     //    if ((result >> 10) & 0x01 == 0) {
//     //      *data += c[0];
//     //      *data += F("WF");
//     //    }
//     //CHECK JAP ERROR                       //JAP ERROR
//     if (((result >> 11) & 0x01) == 0) {
//       //check jap
//       //      *data += c[0];
//       c[1] = WIFI.Internet(0x81);
//       //bit-0 AP
//       if (!(c[1] & 0x01))
//         * data += F(",JF");
//       //BIT-2 Internet
//       else if (!((c[1] >> 2) & 0x01))
//         *data += F(",IF");
//       //bit-3 DNS
//       //      else if (!((c[1] >> 3) & 0x01))
//       //        *data += F(",DF");
//       //host fail
//       else
//         *data += F(",HF");
//     }
//     //result >> 14 & 0x01//+IPD ERROR       //LINK OR SERVER ERROR
//     if (((result >> 14) & 0x01) == 0) {
//       //+IPD ERROR
//       *data += F(",RF");
//     }
//     //result >> 15 & 0x01//SERVER ERROR     //CONFIG ERROR
//     if (((result >> 15) & 0x01) == 0) {
//       //SERVER REPLY ERROR
//       *data += F(",SF");
//     }
//     //    Serial.println(*data);
//     WIFI.Reply(id, data);
//     //    Serial.println(result, HEX);
//     //    Serial.println(*data);
//     //result >> 12 & 0x01//CIPSEND ERROR    //MODULE ERROR
//     //result >> 13 & 0x01//TERMSEND ERROR   //MODULE ERROR
//     goto ok;
//   }
err:
  Serial.println(F("<title>ERROR"));
  return;

ok:
  // WIFI.Reply(id, &ok);
  Serial.println(F("<title>GetOk"));
  return;
}

void app_listen()
{
  if (Serial.available() > 0)
  {
    digitalWrite(APP, LOW); //APP LED
    char cmd[2];
    String st;
    //->/?CD?<data> \r\n
    if (!_Serial.CheckCmd(F(""), F("/?"), ZF0E0, tout1))
    {
      // fail(APP);
      Serial.println(F("CMD ERROR"));
      return;
    }
    Serial.println(F("OK"));
    //EXTRACT CMD
    cmd[0] = Serial.read();
    cmd[1] = Serial.read();
    Serial.read();
    if (cmd[0] == 0xFF || cmd[1] == 0xFF)
    {
      Serial.println(F("CMD not found"));
      return;
    }
    // Serial.println(cmd[0]);

    //EXTRACT DATA UP TO SPACE
    st = _Serial.ReadUpto(F(" "), 1, 0);
    // Serial.println(st);
    // Serial.print(F("len= "));
    // Serial.println(LengthOf(&st));
    // Serial.print(F("?Count= "));
    // Serial.println(CountOf(&st, '?'));
    app(&cmd[0], &st);
    // Eeeprom_SHOW();
    // mod_show_para();
    return;
  }
}

void update_var()
{
  mod_parity_var = get_mod_parity();
  mod_baud_100 = get_mod_baud();
  get_upload_var = get_upload();
  u_retry_var = get_u_retry();
  mod_retry_var = get_mod_retry();
  client_id_var = get_client_id();
  device_id_var = get_device_id();
}

void check_status(uint8_t rst)
{
  //check SIM
  if (!SIM())
  {
    Serial.println(F("SIM ERROR"));
    LED_BLINK->setOverflow(200000UL, MICROSEC_FORMAT);
    LED_BLINK->attachInterrupt(LED_TOGGLE_IT);
    Serial.println(freeRam());
    if (rst == 1)
      GSM.Restart(RST);
  }
  else
  {
    LED_BLINK->detachInterrupt();
    // LED(OFF);
    Serial.println(F("SIM Detected"));
  }
  //check network
  rst = (GSM.Getuint8(F("AT+CREG?"), 2));
  if (rst == 0)
  {
    Serial.println(F("ERROR: Network not Searching"));
  } //Serial.println(GSM.Test(NWtest));}
  else if (rst == 1)
  {
    Serial.println(F("Network Registered"));
  }
  else if (rst == 2)
  {
    Serial.println(F("Network Searching..."));
  }
  else if (rst == 3)
  {
    Serial.println(F("Network Registration denied"));
  }
  else if (rst == 4)
  {
    Serial.println(F("Network Unknown"));
  }
  else if (rst == 5)
  {
    Serial.println(F("Network Registered, Roaming"));
  }
  if ((rst == 1) || (rst == 5))
  {
    Serial.print(F("S/g: "));
    //show signal
    Serial.print(GSM.GetSignal(SigPct));
    Serial.print(F("%"));
    Serial.println();
  }
  //check internet
}

void setup()
{
  // put your setup code here, to run once:
  IWatchdog.reload(); //RESET WATCHDOG
  //wdt reset count
  uint16_t wdtrst = Read16Int(1022);
  if (IWatchdog.isReset(false))
  {
    Write16Int(1022, ++wdtrst);
  }
  IWatchdog.clearReset();
  IWatchdog.begin(WDTus);

  // while(!Serial){wdt_reset();}
  Serial.begin();
  // delay(100);
  _Serial.println(F("Hi"));
  beginRS485();

  GSM_SERIAL.begin(gsmbaud); //GSM

  //LED
  pinMode(L_RED, OUTPUT);
  pinMode(L_GREEN, OUTPUT);
  pinMode(L_YELLOW, OUTPUT);

  LED(OFF);
  if (EEPROM.read(1020) == 0xFF)
  {
    EEPROM.update(1020, 0xFE);
  }
  if (EEPROM.read(1020) == 0)
  {
    Eeeprom_UPDATE();
    EEPROM.update(1020, 1);
  }

  update_var();

  //SET TIMER FOR STARTUP BLINKING
  LED_BLINK->setOverflow(500000UL, MICROSEC_FORMAT);
  LED_BLINK->attachInterrupt(LED_TOGGLE_IT);
  LED_BLINK->resume();

  //SET TIMER FOR STARTUP BLINKING
  Command->setOverflow(1001000UL, MICROSEC_FORMAT);
  Command->attachInterrupt(app_listen);
  Command->resume();

  if (!SIM())
  {
    LED_BLINK->setOverflow(200000UL, MICROSEC_FORMAT);
    Serial.println(freeRam());
    GSM.Restart(RST);
  }
  else
  {
    LED_BLINK->detachInterrupt();
    LED(OFF);
    digitalWrite(L_RED, ON);
    Serial.println(F("Sim Detected"));
  }

  GSM_Heal();
  //wait for network to register
  if (!GSM.Test(NWtest))
  {
    Serial.println(F("Network ERROR"));
  } //Serial.println(GSM.Test(NWtest));}
  else
  {
    Serial.println(F("Network OK"));
  }
  digitalWrite(L_GREEN, ON);
  Serial.println(GSM.Test(NWtest));

  //wait for GPRS to be connacted
  // if(GSM.Internet()){digitalWrite(L_YELLOW, ON);}
  LED(OFF);
  // Eeeprom_SHOW();
}

void loop()
{
  // put your main code here, to run repeatedly:
  // if(Serial.available()){Serial.println(Serial.readString());}

  //read data from modbus
  // Serial.println(read_data_DB());
  // generate data frame
  // data_frame(read_data_DB());

  // put your main code here, to run repeatedly:
  uint32_t mod_timer; // = 0;//(millis() / RATIO_100);
  uint32_t status_timer;
  // uint8_t m100_Sec_upload;// = (EEPROM.read(UPLOAD_d));//d*5=sec,/10
  mod_timer = (millis() / RATIO_100) - (12 * UPLOAD_MULT);
  status_timer = (millis() / 1000) - 100;
  LED(OFF);
  // done:
  // m100_Sec_upload = (get_upload());
  // if (!GSM.AT()){setting();}

  while (1)
  {
    wdt_reset();
    digitalWrite(APP, ON); //APP LED
    // app_listen();
    if (((millis() / 1000) - status_timer) >= (STATUS_UPDATE_TIME))
    {
      check_status(0);
      status_timer = (millis() / 1000);
    }
    //############################ Modbusread and upload
    if (((millis() / RATIO_100) - mod_timer) >= ((get_upload_var * UPLOAD_MULT) - UPLOAD_OFFSET))
    { //*5 //upload timer
      //  Method-1
      //      Serial.println("Modbus");
      digitalWrite(APP, OFF); //APP LED
      MOD_fail_reset();
      mod_timer = (millis() / RATIO_100);
      uint16_t len = read_data_DB();
      if (len > 0)
      {
        //        Serial.println(upload_enc_buff(len));//SAVE+ //RAM EFFICIENT//16B PROM
        // data_frame(len);
        //check status
        check_status(1);
        //check internet
        //UPLOAD_ENC_DATA(SERVER, TYPE_ABP, len);
        UPLOAD_ENC_DATA(SERVER, TYPE_QINN, len);
        // clear heap
        FreeHeap();
        //            Serial.println((millis() - upload_time));//save+//32B PROM
      }
      else
      {
        LED_BLINK->setOverflow(400000UL, MICROSEC_FORMAT);
        LED_BLINK->attachInterrupt(MOD_failed);
      }
      freeRam(); //save-
      wdt_reset();
      //      Serial.println();

      //      ram();//change-//save+//194B
      //      goto done;//CAUSED APP PROBLEM
    }
  }
}
