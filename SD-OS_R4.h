/****************************************************************************/
/*  SD-Card configuration                                                   */
/*                                                                          */
/*                                            Achim Jenne 22.10.2025        */
/****************************************************************************/
#ifndef SDOS_R4
#define SDOS_R4

#define C_BS  0x08
#define C_ESC 0x1b
#define S_CLS F("\e[0H\e[2J")
#define S_CR  F("\r\n>")
#define ILINE 80

// bool beginTimer(float);
// bool editLine(char *, char );

#define SDCRD SPI_FULL_SPEED,PIN_SPI_CS
#endif