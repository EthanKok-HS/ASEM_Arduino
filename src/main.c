#include "global.h"

#define FLICKER_DELAY 1000
#define READ_DELAY    50

#define READ_PIN        5
#define TOGGLE_LED_PIN  7

volatile uint64_t lastFlickerTime = 0;
volatile uint64_t lastReadTime = 0;

int main(void) {
  SYS_Error_Check(SYS_Init());

  uint8_t toggle = 0;
  uint8_t lastState = GPIO_Read(READ_PIN);
  uint32_t count = 0;

  for (;;) {
    if (SYS_TICK - lastFlickerTime > FLICKER_DELAY) {
      if (toggle) {
        SYS_Error_Check(GPIO_Write(LED_DEBUG_PIN, GPIO_HIGH));
        toggle = 0;
      }
      else {
        SYS_Error_Check(GPIO_Write(LED_DEBUG_PIN, GPIO_LOW));
        toggle = 1;
      }

      lastFlickerTime = SYS_TICK;
    }

    if (SYS_TICK - lastReadTime > READ_DELAY) {
      uint8_t currentState = GPIO_Read(READ_PIN);
      if (currentState != lastState) {
        if (currentState) {
          GPIO_Toggle(TOGGLE_LED_PIN);

          count++;
          uartPrint("Count: ");
          uartPrintInt32(count);
          uartPrint("\r\n");
        }
        lastState = currentState;
      }
      lastReadTime = SYS_TICK;
    }

  }
}
  