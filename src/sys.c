#include "global.h"

volatile uint8_t buttonState, lastButtonState = 0;
volatile uint64_t lastDebounceTime = 0;

int32_t _pinInterupt_Init(void);

void SYS_fatal_error(int32_t err) {
  DDRB |= 0x20;
  PORTB |= 0x20;
  uartPrint("Error: ");
  uartPrintInt32(err);
  for(;;);
}

int32_t SYS_Init(void) {

  SYS_Error_Check(TIMER2_Init());
  SYS_Error_Check(TIMER0_Init());
  SYS_TICK = 0;
  sei();

  uartInit(9600);
  uartPrintln("Started");

  SYS_Error_Check(ADC_Init());
  //SYS_Error_Check(I2C_Init());
  //SYS_Error_Check(_pinInterupt_Init());
  SYS_Error_Check(GPIO_Init());
  SYS_Error_Check(GPIO_Mode(LED_DEBUG_PIN, GPIO_OUTPUT));

  SYS_Error_Check(SWUART_Init());

  return SYS_SUCCESS;
}

int32_t _pinInterupt_Init(void) {
  DDRB |= 0x20;
  EIFR |= 0x01;
  EICRA |= 0x03;
  EIMSK |= 0x01;
  return SYS_SUCCESS;
}

// ISR(INT0_vect) {
//   if (SYS_TICK - lastDebounceTime > SYS_DEBOUNCE_DELAY) {
//     PINB |= 0x10;
//     lastDebounceTime = SYS_TICK;
//   }
// }