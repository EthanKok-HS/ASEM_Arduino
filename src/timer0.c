#include "global.h"

int32_t TIMER0_Init(void) {
    TCCR0A = 0x0A; // Enable CTC Mode
    TCCR0B = 0x03; // Clock source set to CLKIO/64 (from prescaler)

    TCNT0 = 0;  // Clear counter 
    OCR0A = 249; // Set reload register to 249

    TIFR0 = 0x07;   // Reset interrupt flags, cleared by writing a logic one to the flag
    TIMSK0 |= 0x02; // Enable output compare match A

    return TIMER0_SUCCESS;
}

ISR(TIMER0_COMPA_vect) {SYS_TICK++;}