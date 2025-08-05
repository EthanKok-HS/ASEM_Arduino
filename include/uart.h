#pragma once

#define UART_SUCCESS 0

#define BILLION 1000000000
#define ASCII_NUMERIC_BASE 48

extern void uartInit(uint32_t baud);
extern void uartPrint(const uint8_t* str);
extern void uartPrintln(const uint8_t* str);

extern void uartPrintInt32(int32_t str);