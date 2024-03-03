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

/*


*/
#define serial  115200

//GSM
//#define GSM Serial//sim800l_test//Serial2
#define gsmbaud 19200//9600
#define rst 11//gsm rst pin
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
#define SigPct    0b00000000
#define Sigusdb     0b00000001
#define Sigsdb      0b10000001

#include <BhanuGSM.h>

BhanuGSM GSM(Serial2);//type-Auto-0 OR GSM-1 OR ESP-2 OR ETHERNET-3 OR LORA-4 etc

const uint16_t baud[10] = {3, 12, 24, 48, 96, 192, 384, 576, 7488, 1152}; //* 100

String st;
//baud scanner
void setup() {
  // put your setup code here, to run once:
  boot();
  Serial.begin(serial);
  Serial2.begin(gsmbaud);
  Serial.println(F("Hi Checking GSM"));
  GSM.Heal();

  GSM.CheckCmd(F("AT+CCLK?"), F("OK"), ZF3Eo1, tout1);
  GSM.CheckCmd(F("AT+SAPBR=4,1"), F("OK"), ZF3Eo1, tout1);
  //  Serial.println(Serial2.readString());
  Serial.println(st);
}
