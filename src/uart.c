#include <util/delay.h>
#include "global.h"

void _uartTransmit(uint8_t data);

void uartInit(uint32_t baud) {
    #define UBRR_VALUE ((F_CPU / 16 / baud) - 1)

    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    UCSR0B = 0x18; // (1 << TXEN0);
    UCSR0C = 0x06; // (1 << UCSZ01) | (1 << UCSZ00);

    UART_TX_BUF_WR = 0; 
    UART_TX_BUF_RD = 0;
    UART_RX_BUF_WR = 0;
    UART_RX_BUF_RD = 0;
    
    UART_TX_BYTES = 0;
    UART_RX_BYTES = 0;
}

void uartPrint(const uint8_t* str) {
    while (*str) {
        _uartTransmit(*str);
        str++;
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

int32_t uartWriteString(uint8_t *str, uint8_t size) {
    while (size > UART_RX_BUF_SIZE - UART_TX_BYTES) uartProcess();

    for (uint8_t i = 0; i < size; i++) {
        if (UART_TX_BYTES >= UART_TX_BUF_WR) {
            tx_buf[UART_TX_BUF_WR] = *str;
            str++;
            UART_TX_BUF_WR = (UART_TX_BUF_WR + 1) & (UART_TX_BUF_SIZE - 1);
            UART_TX_BYTES++;
        }
    }
    return UART_SUCCESS;
}

int32_t uartRead(uint8_t* byte) {
    if (UART_RX_BYTES == 0) return UART_ERROR_RX_BUF_EMPTY;
    *byte = rx_buf[UART_RX_BUF_WR];
    UART_RX_BUF_WR = (UART_RX_BUF_WR + 1) & (UART_RX_BUF_SIZE - 1);
    UART_RX_BYTES--;

    return UART_SUCCESS;
}

int32_t uartProcess(void) {
    if ((UART_TX_BYTES > 0) && ((UCSR0A & 0x20) > 0)) {
        UDR0 = tx_buf[UART_TX_BUF_RD];
        UART_TX_BUF_RD = (UART_TX_BUF_RD + 1) & (UART_TX_BUF_SIZE - 1);
        UART_TX_BYTES--;
    }

    if (((UCSR0A & 0x80) == 0x80) && (UART_RX_BYTES < UART_RX_BUF_SIZE)) {
        rx_buf[UART_RX_BUF_WR] = UDR0 ;
        UART_RX_BUF_RD = (UART_RX_BUF_RD + 1) & (UART_RX_BUF_SIZE - 1);
        UART_RX_BYTES++;
    }
    
    return UART_SUCCESS;
}

void _uartTransmit(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}