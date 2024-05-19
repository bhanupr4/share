//include necessary ASF headers
#include <AT45DB.h>
#include "spi.h"
//SPI connection declaration
#define _csPin    //put own CS pin macros
#define _pageAddrShift  1

//Dataflash commands
#define FlashPageRead           0xD2    // Main memory page read
#define StatusReg               0xD7    // Status register
#define ReadMfgID               0x9F    // Read Manufacturer and Device ID
#define PageErase               0x81    // Page erase
#define ReadSecReg              0x77    // Read Security Register

#define FlashToBuf1Transfer     0x53    // Main memory page to buffer 1 transfer
#define Buf1Read                0xD4    // Buffer 1 read
#define Buf1ToFlashWE           0x83    // Buffer 1 to main memory page program with built-in erase
#define Buf1Write               0x84    // Buffer 1 write

#define FlashToBuf2Transfer     0x55    // Main memory page to buffer 2 transfer
#define Buf2Read                0xD6    // Buffer 2 read
#define Buf2ToFlashWE           0x86    // Buffer 2 to main memory page program with built-in erase
#define Buf2Write               0x87    // Buffer 2 write

void AT45DB_init(void)
{
  //Call standard SPI initialization
  //put spiinit function() here

  //Configire CS pin as OUTPUT
  //Put standard gpio function to set CS pin as OUTPUT

  //_pageAddrShift = 1
}

void en_tx(void) {
  //put SPI start tx api --> set SPI registers for tx
  //Put gpio api to make CS->LOW
}

void dis_tx(void) {
  //Put gpio api to make CS->HIGH
  //put SPI stop tx api --> end SPI tx
}

uint8_t tx(uint8_t data) {
  rerturn ;//Put SPI function to transfer byte
}

uint8_t AT45DB_status(void) {
  uint8_t res;
  en_tx();
  res = transmit(StatusReg);
  res = transmit(0x00);
  dis_tx();

  return res;
}

void AT45DB_wait(void) {
  while (!(AT45DB_status() & 0x80));
}
