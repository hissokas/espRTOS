#include <stdio.h>
#include <string.h>
#include "sd.h"
#include "fat32.h"
#include "usart.h"

#define FirstDataCluster 2

/***************************************finding_BootSector*********************************************************/ 
void finding_BootSector(unsigned char *buffer)
{
 unsigned long index = 0;
 while(1)
 {
  sd_read_sector(index,buffer);
  if((buffer[0] == 0xEB && buffer[2] == 0x90) || (buffer[0] == 0xE9))
   {
    BootSectorAddr = index;
    break;
   }
  index++;
 }
}
/******************************************************************************************************************/




/**************************************read_fat_info****************************************************************/
void read_fat_info(unsigned char *buffer)
{
 finding_BootSector(buffer);
 sd_read_sector(BootSectorAddr, buffer);             // odczytanie wartości BootSector
 
 struct fat_info *info = (struct fat_info*) &buffer[11]; //przekazanie inforamcji z BootSectora do struktury fat_info

 BytesPerSector = (info->bytesPerSector);
 SectorPerCluster = (info->sectorPerCluster);
 ReservedSector = (info->reservedSectorCount);
 RootDirAddr = (BootSectorAddr + (info->reservedSectorCount) + (info->numberofFATs * info->FATsize_F32)); //1 sektor   														RootDirector
 FatSizeInSec = (info->FATsize_F32);
}
/******************************************************************************************************************/




/***************************************FirstClusterSector*********************************************************/
unsigned long FirstClusterSector(unsigned long cluster)
{                                
 return (RootDirAddr + ((cluster-2)*SectorPerCluster));
} 
/******************************************************************************************************************/




/***************************************FileList*******************************************************************/
unsigned long NextCluster(unsigned long cluster, unsigned char *buffer)
{
 unsigned long fat_in_sec;
 unsigned long sector_in_fat;
 unsigned int offset_in_fat;
 unsigned long *NextClust;


  /*******************fat_in_sector****************************/
  fat_in_sec = cluster*4;  //4B oznaczają 1 klaster w Fat
  /********************sector_in_fat***************************/

  sector_in_fat = BootSectorAddr + ReservedSector + (fat_in_sec / BytesPerSector) ;

 /************************offset_inf_fat************************/
 offset_in_fat = (unsigned int) (fat_in_sec % BytesPerSector);   
 
 sd_read_sector(sector_in_fat,buffer);

 NextClust = (unsigned long*)&buffer[offset_in_fat];
return ((*NextClust) & 0x0fffffff);
}
/******************************************************************************************************************/



/***************************************FilesNames*****************************************************************/
void FilesNames(unsigned long sector, unsigned char *buffer)
{

unsigned char i;
unsigned char nr;

 sd_read_sector((RootDirAddr + (sector-1)), buffer);

 for(nr=0;nr<nFilesIndex;nr++)
 {
  for(i=0;i<11;i++)
  FilesName[nr][i] = buffer[((nr*32)+i)]; /*USART_Transmit(buffer[((nr*32)+i)]);*/

 }
}
/******************************************************************************************************************/




/********************************FileList**************************************************************************/

char FileList(unsigned char *buffer)
{
 unsigned long index, index2; 
 /*unsigned char name[11],j*/;
 FileIndex=0;
 NextClus=RootDirCluster;
 
 while(1)
 { 
  for(index2=0; index2<SectorPerCluster; index2++)
  { 
   sd_read_sector((RootDirAddr+index2),buffer);
   for(index=0; index<BytesPerSector; index+=32)
   {
    struct DirStruct *DS = (struct DirStruct*)&buffer[index]; 
    if((DS->name[0] != 0xE5) && (DS->name[0] != 0x00) && (DS->attrib != 0x0F))
    {
	FileIndex++;/*
     for(j=0; j<11; j++)
     name[j]= (DS->name[j]);
     transmitString(name);
	 TX_NEWLINE;*/
    }
   } 
  }
  NextClus=(NextCluster(NextClus,buffer));
  if(NextClus >= 0x0FFFFFF8) return 0;
  RootDirAddr = (FirstClusterSector(NextClus));
 }
} 
/******************************************************************************************************************/
 
 



/********************************FindFile**************************************************************************/
unsigned long FindFileNr(unsigned int FileNr,unsigned char *name_buffer, unsigned char *buffer)
{
 unsigned long index, index2;
 unsigned long FirstCluster;
 //unsigned long index3=0;
 unsigned int index4=0;
 unsigned char in/*, index0*/;
 /*char index_txt=0;
 char position=3;*/
 NextClus=RootDirCluster;

 while(1)
 { 
  for(index2=0; index2<SectorPerCluster; index2++)
  { 
   sd_read_sector((RootDirAddr+index2),buffer);
   for(index=0; index<BytesPerSector; index+=32)
   {
    struct DirStruct *dir = (struct DirStruct*)&buffer[index]; 
     struct LongName  *ptr2 = (struct LongName*)&buffer[index];    
 
    if((ptr2->nr != 0xE5) && (ptr2->nr != 0x00) && (dir->attrib != 0x0F))
    {
     index4++;
    }

	if(index4 == FileNr)
	{
	 FirstCluster |=(dir->firstClusterHI);
	 FirstCluster<<=16;
	 FirstCluster |=(dir->firstClusterLO);
            for(in=0;in<11;in++)
            FileName[in] = dir->name[in]; // FileName -> krótka nazwa pliku
		/***********************************wyswietlanie nazwy*******************************/

		/**************************************************************************************/
         return FirstCluster;
        }
   } 
  }
  NextClus=(NextCluster(NextClus,buffer));

  if(NextClus >= 0x0FFFFFF8) return 0;
  RootDirAddr = (FirstClusterSector(NextClus));
 }
 read_fat_info(buffer);
} 




/******************************************************************************************************************/




/********************************ReadFile**************************************************************************/
char ReadFile(unsigned int FileNr,unsigned char *name_buffer, unsigned char *buffer)
{
 unsigned long NextClus;
 /*unsigned int index=0;*/
 unsigned long index1;
 unsigned long FirstSector;

 char users;
 /*char index_txt=0;
 char position=0;
 int index0;*/
 /*ReadLongFileName(FileNr, name_buffer, buffer);
 read_fat_info(buffer);*/
 NextClus = FindFileNr(FileNr, name_buffer, buffer);
  read_fat_info(buffer);

 while(1)
 {
  FirstSector = FirstClusterSector(NextClus);
  
  for(index1=0; index1<SectorPerCluster; index1++)
  {
   sd_read_sector((FirstSector+index1),buffer); 
    //// odczytany sektor pliku w buffer :)

 }	
  NextClus=NextCluster(NextClus,buffer);
  if(NextClus >= 0x0FFFFFF8) return 0;
 }
}

/******************************************************************************************************************/
 



/******************************************************************************************************************/
void clrBuff(unsigned char *buffer, unsigned char size)
{
 while((size--))
 buffer[size] = 0x00;
}
/******************************************************************************************************************/




/******************************************************************************************************************/
char ReadLongFileName(unsigned int FileNr, unsigned char *name_buffer, unsigned char *buffer)
{
 unsigned long index, index2;
 unsigned long index3=0;
 unsigned int index4_b=0;
 NextClus=RootDirCluster;

 while(1)
 { 
  for(index2=0; index2<SectorPerCluster; index2++)
  { 
   sd_read_sector((RootDirAddr+index2),buffer);
   for(index=0; index<BytesPerSector; index+=32)
   {
    struct DirStruct *dir = (struct DirStruct*)&buffer[index]; 
     struct LongName  *ptr2 = (struct LongName*)&buffer[index];    
if( ((ptr2->nr) != 0xE5) && ((ptr2->nr) != 0x20) && ((dir->attrib) != 0x0F) && ((dir->name[6]) != '~') ) 
      index4_b++;    
    if(((dir->attrib) == 0x0F) && ((ptr2->nr) != 0xE5))
    {
    if( (((ptr2->nr) >= 0x41) && ((dir->attrib) == 0x0F)) ) 
      index4_b++; 
      if(index4_b == FileNr)
       {  
      	      if(((ptr2->nr) >= 0x41) && ((dir->attrib) == 0x0F)) index3=0;
    	 
    	    name_buffer[(index3+=1)] = (ptr2->letter13a);
    	    name_buffer[(index3+=1)] = (ptr2->letter12a);
    	    name_buffer[(index3+=1)] = (ptr2->letter11a);
    	    name_buffer[(index3+=1)] = (ptr2->letter10a);
    	    name_buffer[(index3+=1)] = (ptr2->letter9a);
    	    name_buffer[(index3+=1)] = (ptr2->letter8a);   
    	    name_buffer[(index3+=1)] = (ptr2->letter7a);
    	    name_buffer[(index3+=1)] = (ptr2->letter6a);
    	    name_buffer[(index3+=1)] = (ptr2->letter5a);
    	    name_buffer[(index3+=1)] = (ptr2->letter4a);
    	    name_buffer[(index3+=1)] = (ptr2->letter3a);
    	    name_buffer[(index3+=1)] = (ptr2->letter2a);    
    	    name_buffer[(index3+=1)] = (ptr2->letter1a);   
    	    name_index = index3;
	  }
    	}
     }
   } 
  NextClus=(NextCluster(NextClus,buffer));

  if(NextClus >= 0x0FFFFFF8) return 0;
  RootDirAddr = (FirstClusterSector(NextClus));
 }

}
/******************************************************************************************************************/




/******************************************************************************************************************/
char CreateRootFile(unsigned char name[11], unsigned char *buffer)
{
int index, index1;
char b_control = 0;
unsigned long FreeSpaceSector = 0;
unsigned long RootDirClus = RootDirCluster;
unsigned long RootAddr = RootDirAddr;
unsigned char offset=0;
unsigned char *pointer;

/***************************Skladowe rootDir******************/
unsigned int time_crt = 0xBF7D; //23:58:58
unsigned int date_crt = 0x1903; //03.08.1992
unsigned int date_acc = 0x2821; //01.01.2000
unsigned int time_lwr = 0xBF7D; //23:58:58
unsigned int date_lwr = 0x2821; //01.01.2000
unsigned long size = 0;//BytesPerSector*SectorPerCluster;

unsigned long cluster = FreeClusInFat(buffer);
/***************************FatFileName***********************/
 unsigned char  NAME[11];
 unsigned char  CHAR,Cindex;
 unsigned char Csize = strlen(name);
 Cindex = 0;
 while(Cindex != 11)
 {
  CHAR = *name;

  if(CHAR=='.')
  { 
   CHAR=' ';
   Csize = (11-Csize);
   while(Csize--) NAME[Cindex++]=' ';
  }
  name++;
  NAME[Cindex++] = CHAR;
 }
/**************************************************************/
if(FindFile(NAME,buffer) != 0) return 1;  // sprawdzanie czy plik o danej nazwie istnieje
if(cluster == 0) return 1; // sprawdzanie czy jest jakieś wolne miejsce na partycji

/***************************Dane rootDir***********************/
char RootFile[32];
memcpy(RootFile, NAME, 11);
RootFile[11] = 0x00;
RootFile[12] = 0x00; 
RootFile[13] = 0x00;
RootFile[14] = (char)time_crt;
RootFile[15] = (char)(time_crt>>8)&0xFF;
RootFile[16] = (char)date_crt;
RootFile[17] = (char)(date_crt>>8)&0xFF;
RootFile[18] = (char)date_acc;
RootFile[19] = (char)(date_acc>>8)&0xFF;
RootFile[20] = (char)(cluster>>16)&0xFF;
RootFile[21] = (char)(cluster>>24)&0xFF;
RootFile[22] = (char)time_lwr;
RootFile[