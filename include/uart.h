#pragma once

#define UART_ERROR_BASE 0x00009000
#define UART_SUCCESS 0
#define UART_ERROR_RX_BUF_EMPTY (UART_ERROR_BASE + 1)
#define UART_ERROR_TX_BUF_FULL (UART_ERROR_BASE + 2)

#define BILLION 1000000000
#define ASCII_NUMERIC_BASE 48

#define UART_TX_BUF_SIZE 32
#define UART_RX_BUF_SIZE 32

uint8_t tx_buf[UART_TX_BUF_SIZE];
uint8_t rx_buf[UART_RX_BUF_SIZE];

uint8_t UART_TX_BUF_WR, UART_TX_BUF_RD, UART_TX_BYTES; 
uint8_t UART_RX_BUF_WR, UART_RX_BUF_RD, UART_RX_BYTES; 

extern void uartInit(uint32_t baud);
extern void uartPrint(const uint8_t* str);
extern void uartPrintln(const uint8_t* str);
extern void uartPrintInt32(int32_t str);

extern int32_t uartWriteString(uint8_t *str, uint8_t size);
extern int32_t uartRead(uint8_t* byte);
extern int32_t uartProcess(void);