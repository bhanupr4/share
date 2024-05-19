#ifndef AT45DB_H
#define AT45DB_H

#define DF_AT45DB081D   1
#define DF_AT45DB161D   2
#define DF_AT45DB041D   3
#define DF_AT45DB321D   4

#elif DF_VARIANT == DF_AT45DB041D
// configuration for the Atmel AT45DB041D device
#define DF_PAGE_ADDR_BITS       13
#define DF_PAGE_SIZE            512
#define DF_PAGE_BITS            9

#define DF_NR_PAGES     (1 << DF_PAGE_ADDR_BITS)

void init(uint8_t csPin=SS);
void init(uint8_t misoPin, uint8_t mosiPin, uint8_t sckPin, uint8_t ssPin) __attribute__((deprecated("Use: void init(uint8_t csPin=SS)")));
void readID(uint8_t *data);
void readSecurityReg(uint8_t *data, size_t size);

uint8_t readByteBuf1(uint16_t pageAddr);
void readStrBuf1(uint16_t addr, uint8_t *data, size_t size);
void writeByteBuf1(uint16_t addr, uint8_t data);
void writeStrBuf1(uint16_t addr, uint8_t *data, size_t size);

void writeBuf1ToPage(uint16_t pageAddr);
void readPageToBuf1(uint16_t PageAdr);

void pageErase(uint16_t pageAddr);
void chipErase();

void settings(SPISettings settings);


uint8_t readStatus();
void waitTillReady();
uint8_t transmit(uint8_t data);
void activate();
void deactivate();
void setPageAddr(unsigned int PageAdr);
uint8_t getPageAddrByte0(uint16_t pageAddr);
uint8_t getPageAddrByte1(uint16_t pageAddr);
uint8_t getPageAddrByte2(uint16_t pageAddr);



#endif // AT45DB_H
