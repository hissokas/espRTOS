/**************************************************************************************************/
/* Biblioteka została opracowana przez Patryka Łaś                                                */
/* Poniższa biblioteka nie może być publikowana, przetwarzana ani stosowana (w rozwiązaniach      */
/* komercyjnych bądź zarobkowych) bez uprzedniej zgody autora                                     */
/* Wszystkie prawa zastrzeżone                                                                    */
/* Kontakt (laspatryk@wp.pl, www.patlas.elektroda.eu)                                             */
/**************************************************************************************************/

#include <stdio.h>


#include "sd.h"

/////////////////////////////////////////////////////////////////////////////////////////////  
 
void spiSendByte(char dana)
{
   SPDR = dana;
   while((SPSR & 128)!=128);  //czekaj na zakonczenie transmisji
}

//////////////////////////////////////////////////////////////////////////////////////////////
unsigned char spiTransferByte(unsigned char dana)
{
	
   SPDR=dana;
   while((SPSR & 128)!=128);  //czekaj na zakonczenie transmisji
   return SPDR;
} 
 
////////////////////////////////////////////////////////////////////////////////////////////// 
void sd_clear(void)
{  
 char index;
  for(index=0; index<10; index++)
 {
  spiSendByte(0xFF);
 }          
}
/////////////////////////////////////////////////////////////////////////////////////////////// 

int sd_cmd(unsigned char cmd, unsigned long data)
{
 char index;  
 char R1;
 data <<=9;
 spiTransferByte(cmd | 0x40);
 spiTransferByte((data >> 24) & 0xFF);
 spiTransferByte((data >> 16) & 0xFF);    
 spiTransferByte((data >> 8)  & 0xFF);
 spiTransferByte((data >> 0)  & 0xFF);
 spiTransferByte(0x95);   
        

 for(index=0, R1=0xFF; index<10,((R1&128)==128); index++)
 {
  R1 = spiTransferByte(0xFF);
 }
 return R1;
}
/////////////////////////////////////////////////////////////////////////////////////////

char sd_init(void)
{
  char index = 0;
  unsigned char ret;
  
do
{ 
CS_set();
sd_clear();
CS_clr();

ret = sd_cmd(SD_IDLE_STAT,0x00);
if(index > 9) return 1;       
_delay_ms(50);
index++;
} while(ret != R1_IDLE_STAT);

index=0;
                                  
do
{
CS_set();
sd_clear();
CS_clr();
ret = sd_cmd(SD_SEND_OP_COND,0x00);
if(index > 100) return 2;
_delay_ms(5);
index++;      
} while(ret); 

sd_cmd(SD_CRC,0x00); 	  
sd_cmd(SD_SET_BLOCKLEN, 0x200); 
return 0;
} 

///////////////////////////////////////////////////////////////////////////////////////////
char card_init(void)
{
 char i=0;
 while(sd_init() != 0 && i<5)
 i++;
 if(i==5)
  return 1;
 else
  return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////


int sd_write_sector(unsigned long sector, unsigned char *buffer)
{
 unsigned long index; 
 unsigned char ret; 
 char i;
 CS_clr();
 ret= sd_cmd(SD_WRITE_BLOCK, sector); 
 if(ret != 0)
  return 1;//ERROR
 
 
 spiTransferByte(0xFF);
 spiTransferByte(0xFE);
 
 for(index=0; index<512; index++)
 {
  spiTransferByte(buffer[index]);
 }
 spiTransferByte(0xFF);
 spiTransferByte(0xFF); 
 
for(i=0; i<8; i++)
{
if((ret = spiTransferByte(0xFF))!=0xFF)
break;
}


 if((ret&0x1F) != 0x05)
 return 1; //ERROR

while(!spiTransferByte(0xFF));
CS_set();
spiSendByte(0xFF);

return 0;

}
/////////////////////////////////////////////////////////////////////////////////////////////

char sd_read_sector(unsigned long sector,unsigned char *buffer)
{
 unsigned long index; 
 unsigned char ret;   

 
 CS_clr();   

 ret = sd_cmd(SD_READ_SINGLE_BLOCK, sector);
 if(ret !=0x00)
 return 1;
 ret =0;
 while(ret!= 0xFE)
 {
  ret = spiTransferByte(0xFF);
 } 

for(index=0; index<512; index++)
{
 buffer[index] = spiTransferByte(0xFF);
}  
spiTransferByte(0xFF);
spiTransferByte(0xFF);
CS_set();
spiTransferByte(0xFF);  
return 0;
}


 
 
