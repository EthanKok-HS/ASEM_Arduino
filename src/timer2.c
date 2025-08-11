#include "global.h"

int32_t TIMER2_Init(void) {
    TCCR2A = 0x0A; // Enable CTC Mode
    TCCR2B = 0x04; // Clock source set to CLKIO/64 (from prescaler)

    TCNT2 = 0;  // Clear counter 
    OCR2A = 249; // Set reload register to 249

    TIFR2 = 0x07;   // Reset interrupt flags, cleared by writing a logic one to the flag
    TIMSK2 |= 0x02; // Enable output compare match A

    return TIMER2_SUCCESS;
}

ISR(TIMER2_COMPA_vect) {SYS_TICK++;}