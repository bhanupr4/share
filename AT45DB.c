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
  res = tx(StatusReg);
  res = tx(0x00);
  dis_tx();

  return res;
}

void AT45DB_wait(void) {
  while (!(AT45DB_status() & 0x80));
}

void AT45DB_id(uint8_t *id) {
  en_tx();
  tx(ReadMfgID);
  for(uint8_t i = 0; i < 4; i++) {
    id[i] = tx(0);
  }
  dis_tx();
}

void getSecurity(uint8_t *data, size_t size)
{
    en_tx();
    tx(ReadSecReg);
    tx(0x00);
    tx(0x00);
    tx(0x00);
    for (size_t i = 0; i < size; i++) {
      *data++ = transmit(0x00);
    }
    dis_tx();
}

uint8_t getPageAddrB0(uint16_t pageAddr)
{
  return (pageAddr << (DF_PAGE_BITS - 8)) >> 8;
}

uint8_t getPageAddrB1(uint16_t page)
{
  return page << (DF_PAGE_BITS - 8);
}

void setPageAddr(uint16_t pageAddr)
{
  tx(getPageAddrB0(pageAddr));
  tx(getPageAddrB1(pageAddr));
  tx(0);
}
////////////// READ /////////////////
// Transfers a page from flash to Dataflash SRAM buffer
void readPageToBuf1(uint16_t pageAddr)
{
  en_tx();
  tx(FlashToBuf1Transfer);
  setPageAddr(pageAddr);
  dis_tx();
  AT45DB_wait();
}

// Reads one byte from one of the Dataflash internal SRAM buffer 1
uint8_t readByteBuf1(uint16_t addr)
{
  unsigned char data = 0;

  en_tx();
  tx(Buf1Read);
  tx(0x00);               //don't care
  tx((uint8_t) (addr >> 8));
  tx((uint8_t) (addr));
  tx(0x00);               //don't care
  data = tx(0x00);        //read byte
  dis_tx();

  return data;
}

// Reads a number of bytes from one of the Dataflash internal SRAM buffer 1
void readBytesBuf1(uint16_t addr, uint8_t *data, size_t size)
{
  en_tx();
  tx(Buf1Read);
  tx(0x00);               //don't care
  tx((uint8_t) (addr >> 8));
  tx((uint8_t) (addr));
  tx(0x00);               //don't care
  for (size_t i = 0; i < size; i++) {
    *data++ = tx(0x00);
  }
  dis_tx();
}
////////////// WRITE /////////////////
// Writes one byte to one to the Dataflash internal SRAM buffer 1
void writeByteBuf1(uint16_t addr, uint8_t data)
{
  en_tx();
  tx(Buf1Write);
  tx(0x00);               //don't care
  tx((uint8_t) (addr >> 8));
  tx((uint8_t) (addr));
  tx(data);               //write data byte
  dis_tx();
}

// Writes a number of bytes to one of the Dataflash internal SRAM buffer 1
void writeStrBuf1(uint16_t addr, uint8_t *data, size_t size)
{
  en_tx();
  tx(Buf1Write);
  tx(0x00);               //don't care
  tx((uint8_t) (addr >> 8));
  tx((uint8_t) (addr));
  for (size_t i = 0; i < size; i++) {
    tx(*data++);
  }
  dis_tx();
}

// Transfers Dataflash SRAM buffer 1 to flash page
void writeBuf1ToPage(uint16_t pageAddr)
{
  en_tx();
  tx(Buf1ToFlashWE);
  setPageAddr(pageAddr);
  dis_tx();
  AT45DB_wait();
}

////////////// ERASE /////////////////
