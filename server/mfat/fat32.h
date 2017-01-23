#ifndef FAT32_H_INCLUDED
#define FAT32_H_INCLUDED 

#include <avr/io.h>
#include <util/delay.h>
#define RootDirCluster 0x02
#define SW_BACK bit_is_clear(PIND,1)
#define SW_PLAY_PAUSE bit_is_clear(PIND,3)
#define SW_STOP bit_is_clear(PIND,2)
#define SW_NEXT bit_is_clear(PIND,4)
//#define SW_NEXT bit_is_clear(PIND,2)

unsigned long BootSectorAddr;
unsigned long RootDirAddr;
unsigned int BytesPerSector;
unsigned char SectorPerCluster;
unsigned int ReservedSector;
unsigned long NextClus;
unsigned char FileName[11];
unsigned long name_index;
unsigned long FileIndex;
struct fat_info
{

 //unsigned char jumpBoot[3]; //default: 0x009000EB
 //unsigned char OEMName[8];
 unsigned int bytesPerSector; //deafault: 512
 unsigned char sectorPerCluster;
 unsigned int reservedSectorCount;
 unsigned char numberofFATs;
 unsigned int rootEntryCount;
 unsigned int totalSectors_F16; //must be 0 for FAT32
 unsigned char mediaType;
 unsigned int FATsize_F16; //must be 0 for FAT32
 unsigned int sectorsPerTrack;
 unsigned int numberofHeads;
 unsigned long hiddenSectors;
 unsigned long totalSectors_F32;
 unsigned long FATsize_F32; //count of sectors occupied by one FAT
 unsigned int extFlags;
 unsigned int FSversion; //0x0000 (defines version 0.0)
 unsigned long rootCluster; //first cluster of root directory (=2)
 unsigned int FSinfo; //sector number of FSinfo structure (=1)
 unsigned int BackupBootSector;
 unsigned char reserved[12];
 unsigned char driveNumber;
 unsigned char reserved1;
 unsigned char bootSignature;
 unsigned long volumeID;
 unsigned char volumeLabel[11]; //"NO NAME "
 unsigned char fileSystemType[8]; //"FAT32"
// unsigned char bootData[420];
// unsigned int bootEndSignature; //0xaa55
};

struct DirStruct
{
 unsigned char name[11];
 unsigned char attrib; //file attributes
 unsigned char NTreserved; //always 0
 unsigned char timeTenth; //tenths of seconds, set to 0 here
 unsigned int createTime; //time file was created
 unsigned int createDate; //date file was created
 unsigned int lastAccessDate;
 unsigned int firstClusterHI; //higher word of the first cluster number
 unsigned int writeTime; //time of last write
 unsigned int writeDate; //date of last write
 unsigned int firstClusterLO; //lower word of the first cluster number
 unsigned long fileSize; //size of file in bytes
};

struct LongName
{
 unsigned char nr;
 unsigned char letter1a;
 unsigned char letter1b;
 unsigned char letter2a;
 unsigned char letter2b;
 unsigned char letter3a;
 unsigned char letter3b;
 unsigned char letter4a;
 unsigned char letter4b;
 unsigned char letter5a;
 unsigned char letter5b;
 unsigned int hex_0F00;
 unsigned char check_sume;
 unsigned char letter6a;
 unsigned char letter6b;
 unsigned char letter7a;
 unsigned char letter7b;
 unsigned char letter8a;
 unsigned char letter8b;
 unsigned char letter9a;
 unsigned char letter9b;
 unsigned char letter10a;
 unsigned char letter10b;
 unsigned char letter11a;
 unsigned char letter11b;
 unsigned  int hex_0000;
 unsigned char letter12a;
 unsigned char letter12b;
 unsigned char letter13a;
 unsigned char letter13b;
};

void finding_BootSector(unsigned char *buffer);
void read_fat_info(unsigned char *buffer);
unsigned long FirstClusterSector(unsigned long cluster);
unsigned long NextCluster(unsigned long cluster, unsigned char *buffer);
//char FileList(unsigned char *buffer);
void FilesNames(unsigned long sector,unsigned char *name_buffer[], unsigned char *buffer);
unsigned long FindFileNr(unsigned int FileNr,unsigned char *name_buffer, unsigned char *buffer);
char ReadFile(unsigned int FileNr,unsigned char *name_buffer, unsigned char *buffer);
void clrBuff(unsigned char *buffer, unsigned char size);
char ReadLongFileName(unsigned int FileNr, unsigned char *name_buffer, unsigned char *buffer);

#endif
