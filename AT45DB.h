#ifndef AT45DB_H
#define AT45DB_H

#define DF_PAGE_ADDR_BITS       13
#define DF_PAGE_SIZE            512
#define DF_PAGE_BITS            9

#define DF_NR_PAGES     (1 << DF_PAGE_ADDR_BITS)

void AT45DB_init(void);
void en_tx(void);
void dis_tx(void);
uint8_t tx(uint8_t data);
uint8_t AT45DB_status(void);
void AT45DB_wait(void);
void AT45DB_id(uint8_t *id);
void getSecurity(uint8_t *data, size_t size);
uint8_t getPageAddrB0(uint16_t pageAddr);
uint8_t getPageAddrB1(uint16_t page);
void setPageAddr(uint16_t pageAddr);
void readPageToBuf1(uint16_t pageAddr);
uint8_t readByteBuf1(uint16_t addr);
void readBytesBuf1(uint16_t addr, uint8_t *data, size_t size);
void writeByteBuf1(uint16_t addr, uint8_t data);
void writeStrBuf1(uint16_t addr, uint8_t *data, size_t size);
void writeBuf1ToPage(uint16_t pageAddr);
void page_Erase(uint16_t pageAddr);
void chip_Erase();

#endif // AT45DB_H
