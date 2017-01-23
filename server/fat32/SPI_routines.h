//**************************************************************
// ****** FUNCTIONS FOR SPI COMMUNICATION *******
//**************************************************************
//Controller: ATmega32 (Clock: 8 Mhz-internal)
//Compiler	: AVR-GCC (winAVR with AVRStudio)
//Project V.: Version - 2.4.1
//Author	: CC Dharmani, Chennai (India)
//			  www.dharmanitech.com
//Date		: 24 Apr 2011
//**************************************************************

#ifndef _SPI_ROUTINES_H_
#define _SPI_ROUTINES_H_

#include "../driver/spi.h"

#define SPI_SD             spi_clock(HSPI, 16 , 10)// 80 / (5*8) MHz
#define SPI_HIGH_SPEED     spi_clock(HSPI, 8, 5)// 80 / (2*1) MHz // TODO change if does not work


void spi_init2(void);
#define SPI_transmit(x) spi_rxtx8(HSPI, x)
#define SPI_receive() spi_rxtx8(HSPI, 0xFF)

#endif
