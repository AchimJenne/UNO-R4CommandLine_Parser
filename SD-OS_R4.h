/****************************************************************************/
/*  SD-Card configuration                                                   */
/*                                                                          */
/*                                            Achim Jenne 22.10.2025        */
/****************************************************************************/
#ifndef SDOS_R4
#define SDOS_R4

//XModem
#define SOH  0x01 // 128 byte
#define STX  0x02 // 1024 byte
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define PAD  0x1a
#define X_BLOCK_SIZE  128
#define X_BUF_SIZE    X_BLOCK_SIZE + 6 
#define X_RETRANS 10

#define C_BS  0x08
#define C_ESC 0x1b
#define S_CLS F("\e[0H\e[2J")
#define S_CR  F("\r\n>")
#define ILINE 80

// bool beginTimer(float);
// bool editLine(char *, char );

//#define SDCRD SPI_FULL_SPEED,PIN_SPI_CS
#define SDCRD PIN_SPI_CS
#endif