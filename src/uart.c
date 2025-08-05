#include <util/delay.h>
#include "global.h"

void _uartTransmit(uint8_t data);

void uartInit(uint32_t baud) {
    #define UBRR_VALUE ((F_CPU / 16 / baud) - 1)

    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    UCSR0B = 0x18; // (1 << TXEN0);
    UCSR0C = 0x06; // (1 << UCSZ01) | (1 << UCSZ00);
}

void uartPrint(const uint8_t* str) {
    while (*str) {
        _uartTransmit(*str++);
    }
}

void uartPrintln(const uint8_t* str) {
    uartPrint(str);
    _uartTransmit('\r');
    _uartTransmit('\n');
}

void uartPrintInt32(int32_t str) {
    if (str < 0) _uartTransmit('-');

    // 32-bit integer has a max 10-digit integer, hence 10^9
    if (str / BILLION) _uartTransmit(ASCII_NUMERIC_BASE + (str / BILLION));
    uint8_t firstDigitPrinted = 0;
    for (uint8_t i = 1; i <= 9 ; i++) {

        uint32_t mod = 1;
        for(uint8_t j = i; j <= 8; j++) mod *= 10; // Multiply one time less here to prevent additional division later
        uint8_t digit = (str % (mod * 10)) / mod;

        if (digit || firstDigitPrinted) {
            firstDigitPrinted = 1;
            _uartTransmit(ASCII_NUMERIC_BASE + digit);
        }
    }    
}

void _uartTransmit(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}