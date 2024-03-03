/*
  //AT+HTTPSCONT Save HTTP Application Context
Load data like URL, CID, UA
*/
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

///////////////////////

#include<BhanuEEPROM.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

  WriteeepString(AT_, HTTP - 1, F("AT"));
  WriteeepString(HTTP, TERM - 1, F("HTTP"));


  Serial.println(ReadeepString(AT) + '+' + ReadeepString(HTTP));
}

void loop() {
  // put your main code here, to run repeatedly:

}
