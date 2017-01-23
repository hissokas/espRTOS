/**************************************************************************************************/
/* Biblioteka została opracowana przez Patryka Łaś                                                */
/* Poniższa biblioteka nie może być publikowana, przetwarzana ani stosowana (w rozwiązaniach      */
/* komercyjnych bądź zarobkowych) bez uprzedniej zgody autora                                     */
/* Wszystkie prawa zastrzeżone                                                                    */
/* Kontakt (laspatryk@wp.pl, www.patlas.elektroda.eu)                                             */
/**************************************************************************************************/
#ifndef SD_H_INCLUDED
#define SD_H_INCLUDED
//COMMAND FOR SD
#define SD_IDLE_STAT 0x00
#define SD_SEND_OP_COND 0x01
#define SD_SET_BLOCKLEN 0x10
#define SD_READ_SINGLE_BLOCK 0x11
#define SD_WRITE_BLOCK 0x18
#define SD_CRC 59
//ANSWERS FROM SD
#define R1_BUSY 0x80
#define R1_IDLE_STAT 0x01     
#define R1_OP_COND 0x00

// ANOTHER FUNCTION OF PROGRAMM

#define CS_set() (PORTB |= _BV(3))
#define CS_clr() (PORTB &= ~_BV(3))  

void spiSendByte(char dana);  //funkcja wysyla dana poprzez SPI
unsigned char spiTransferByte(unsigned char dana);//funkcja wysyla dana i zwraca odebrana przez SPI
char card_init(void);
char sd_init(void);
void sd_clear(void);
int sd_cmd(unsigned char cmd, unsigned long data);
int sd_write_sector(unsigned long sector, unsigned char *buffer);
char sd_read_sector(unsigned long sector, unsigned char *buffer);
#endif

