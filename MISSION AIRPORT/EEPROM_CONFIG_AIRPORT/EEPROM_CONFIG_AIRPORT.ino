/*
   EEPROM CONFIGURATION OF GATEWAY-AIRPORT

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

*/
//ip4, ip3, ip2, ip1, port, upload,client, device, capture
const uint16_t add_eeprom[] = {539, 540, 541, 542, 543, 546, 547, 550, 554};
//MODBUS: START_ADD, N-POLL, SLAVE_ID, MOD_FUN,PARITY, BAUD
const uint16_t add_modbus[] = {4098, 4098, 4098, 4098, 555, 556}; //2-EVEN
String ip_var, port_var, client_id_var, device_id_var;
uint8_t upload = 6, capture_id_var = 1; //uload time
uint16_t start_address_var;
uint8_t slave_id_var, mod_fun_var, mod_parity_var, mod_retry_var;
uint8_t npoll_var[2];
uint32_t mod_baud_var;

#include<EEPROM.h>

void wdt_reset()
{

}


////////////////////// READ EEPROM ////////////
String readeepstring(uint16_t i)//i- starting address
{ //--//Serial.println(F("READING EEPROM String"));
  wdt_reset();
  String temp;
  for (uint16_t addr = i; ((char)EEPROM.read(addr) != NULL); addr++)
  { temp = temp + ((char)EEPROM.read(addr));
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
  address = address + 1;
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

//bool writeeepstring(uint16_t i, uint16_t l, String temp) //i- initial address l- end address
//{ wdt_reset();
//  if (((l - i) + 1) >= lengthof(temp))
//  { uint16_t add;
//    wdt_reset();
//    //Serial.println(F("memory ok"));
//    //    disable_intr();
//    for (add = 0; temp[add] != NULL; add++)
//    { EEPROM.update((add + i), temp[add]);
//      //Serial.print("add[");
//      //Serial.print(add + i);
//      //Serial.print("]=");
//      //Serial.println((char)EEPROM.read(add + i));
//      //      wdt_reset();
//    }
//    EEPROM.update((add + i), 0);
//    //    enable_intr();
//    return true;
//  }
//  else
//  { //Serial.println(F("memory error"));
//    return false;
//  }
//}
bool writeeepstring(uint16_t i, uint16_t l, String *temp) //i- initial address l- end address
{
  //  wdt_reset();
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
    Serial.println(*temp);
    //    enable_intr();
    return true;
  }
  else
  { //Serial.println(F("memory error"));
    return false;
  }
}

/////////////////////////////////////////
//uint16_t lengthof(String stemp)          //to find length of string
//{ uint16_t len = 0;
//  for (uint16_t i = 0; stemp[i] != NULL; i++)
//  { len++;
//    wdt_reset();
//  }
//  return len;
//}

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
  if (upload <= 1 || upload > 100)
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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  pinMode(13, OUTPUT);
  //  //SET IP
  EEPROM.update(add_eeprom[0], 103); //IP4
  EEPROM.update(add_eeprom[1], 233); //IP3
  EEPROM.update(add_eeprom[2], 79); //IP2
  EEPROM.update(add_eeprom[3], 197); //IP1
  //  //SET PORT
  write16int(add_eeprom[4], 8000); //PORT
  //  //SET UPLOAD TIME
  EEPROM.update(add_eeprom[5], (60 / 5)); //UPLOAD
  write16int(add_eeprom[6], 12); //CLIENT
  write16int(add_eeprom[7], 15); //DEVICE - 15 FOR kial - 20 FOR venlitead
  EEPROM.update(add_eeprom[8], 3); //capture id
  //
  //  //MODBUS
  //  write16int(add_modbus[0], 3840); //START ADDRESS
  //  EEPROM.update(add_modbus[1], 41); //No. OF POLL
  //    EEPROM.update(add_modbus[2], 1); //SLAVE ID
  //  EEPROM.update(add_modbus[3], 3); //MOD_FUNCTION
  EEPROM.update(add_modbus[4], 2); //MOD_PARITY
  EEPROM.update(add_modbus[5], 2); //2-9600
  EEPROM.update(557, 3); //MOD_RETRY
  //  EEPROM.update(15, 64);
  //  EEPROM.update(23, 64);
  //
  //  //SSID AND KEY
  //  String st;
  //  st = F("Venlite");
  //  writeeepstring(100, 132, &st);  //WRITE SSID
  //  st = F("venlite@123");
  //  writeeepstring(133, 165, &st);  //WRITE KEY


  get_ip();
  get_port();
  get_upload();
  get_client_id();
  get_device_id();
  get_capture_id();

  //MODBUS
  //  get_start_address();
  //  get_npoll();
  //  get_slave_id();
  //  get_mod_fun();
  get_mod_parity();
  get_mod_baud();
  get_mod_retry();
}

void loop() {
  // put your main code here, to run repeatedly:

}
