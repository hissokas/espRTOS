//*************************************************************
//*** FUNCTIONS FOR EEPROM READ/WRITE ON I2C COMMUNICATION  ***
//*************************************************************
//Controller: ATmega32 (Clock: 8 Mhz-internal)
//Compiler	: AVR-GCC (winAVR with AVRStudio)
//Project V.: Version - 2.4.1
//Author	: CC Dharmani, Chennai (India)
//			  www.dharmanitech.com
//Date		: 24 Apr 2011
//********************************************************

#include "RTC_routines.h"

unsigned char time[10]; 		//xxam:xx:xx;
unsigned char date[12];		//xx/xx/xxxx;
unsigned char day;

//***************************************************************************
//Function to set initial address of the RTC for subsequent reading / writing
//***************************************************************************
unsigned char RTC_setStartAddress(void)
{
   return 0;
}

//***********************************************************************
//Function to read RTC registers and store them in buffer rtc_register[]
//***********************************************************************    
unsigned char RTC_read(void)
{
	return 0;
}	  

//******************************************************************
//Function to form time string for sending it to LCD & UART
//****************************************************************** 
unsigned char RTC_getTime(void)
{
   return 0;
}

//******************************************************************
//Function to form date string for sending it to LCD & UART
//****************************************************************** 
unsigned char RTC_getDate(void)
{
  return 0;
}  
  
//******************************************************************
//Function to display time on LCD and send it to PC (thru UART)
//****************************************************************** 
unsigned char RTC_displayTime(void)
{
  return 0;
}

//******************************************************************
//Function to display date on LCD and send it to PC (UART)
//****************************************************************** 
unsigned char RTC_displayDate(void)
{
  return 0; 
}

//******************************************************************
//Function to get the string for day 
//****************************************************************** 
void RTC_displayDay(void)
{
  
}	  		  
		  		     	  
//******************************************************************
//Function to update buffer rtc_register[] for next writing to RTC
//****************************************************************** 
void RTC_updateRegisters(void)
{

}  


//******************************************************************
//Function to write new time in the RTC 
//******************************************************************   
unsigned char RTC_writeTime(void)
{
	return(0);
}


//******************************************************************
//Function to write new date in the RTC
//******************************************************************   
unsigned char RTC_writeDate(void)
{
  
	return(0);
}
  
//******************************************************************
//Function to update RTC time by entering it at hyper terminal
//******************************************************************   
unsigned char RTC_updateTime(void)
{
  
  return 0;
}  
  
  
//******************************************************************
//Function to update RTC date by entering it at hyper terminal
//******************************************************************   
unsigned char RTC_updateDate(void)
{
   
  return 0;
}  



//******************************************************************
//Function to get RTC date & time in FAT32 format
//******************************************************************   
unsigned char getDateTime_FAT(void)
{
  
 
   return 0;
}
	  
 
 

 
