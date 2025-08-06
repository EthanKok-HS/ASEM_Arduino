#include "global.h"

#define FLICKER_DELAY 1000
#define READ_DELAY    50

#define READ_PIN        5
#define READ_PIN_2      6
#define TOGGLE_LED_PIN  7

#define MAX_LED_COUNT   4

void heartbeat(void);
void delay(uint32_t delay);
void led_wave(uint32_t wave_count, uint32_t delay, uint8_t rebound);

volatile uint64_t lastFlickerTime = 0;
volatile uint64_t lastReadTime = 0;

int main(void) {
  SYS_Error_Check(SYS_Init());

  uint8_t lastState1 = GPIO_Read(READ_PIN);
  uint8_t lastState2 = GPIO_Read(READ_PIN_2);
  uint32_t count = 0;

  uint32_t led_count = 0;
  uint8_t feedback;

  uartWriteString("Hello World", 11);

  for (;;) {
    heartbeat();
    uartProcess();

    if (uartRead(&feedback) == 0) {
      uartPrint(&feedback);
    }

    if (SYS_TICK - lastReadTime > READ_DELAY) {
      uint8_t currentState = GPIO_Read(READ_PIN);
      if (currentState != lastState1) {
        if (currentState) {
          //GPIO_Toggle(TOGGLE_LED_PIN);

          count++;
          uartPrint("Count: ");
          uartPrintInt32(count);
          uartPrint("\r\n");

          led_wave(2, 100, 1);
        }
        lastState1 = currentState;
      }

      currentState = GPIO_Read(READ_PIN_2);
      if (currentState != lastState2) {
        if (currentState) {
          GPIO_Toggle(TOGGLE_LED_PIN + led_count);
          led_count = led_count >= MAX_LED_COUNT - 1 ? 0 : led_count + 1;

          count++;
          uartPrint("Count: ");
          uartPrintInt32(count);
          uartPrint("\r\n");
        }
        lastState2 = currentState;
      }
      lastReadTime = SYS_TICK;
    }

  }
}

void led_wave(uint32_t wave_count, uint32_t d, uint8_t rebound) {
  
  for (uint32_t loop = 0; loop < wave_count; loop++) {
    for (uint16_t i = 0; i < MAX_LED_COUNT; i++) {
      GPIO_Toggle(TOGGLE_LED_PIN + i);
      delay(d);
      GPIO_Toggle(TOGGLE_LED_PIN + i);
    }

    if (!rebound) continue;
    
    for (int16_t i = MAX_LED_COUNT - 1; i >= 0; i--) {
      GPIO_Toggle(TOGGLE_LED_PIN + i);
      delay(d);
      GPIO_Toggle(TOGGLE_LED_PIN + i);
    }
  }
}

void heartbeat(void) {
  if (SYS_TICK - lastFlickerTime > FLICKER_DELAY) {
    SYS_Error_Check(GPIO_Write(LED_DEBUG_PIN, (GPIO_Read(LED_DEBUG_PIN) ? GPIO_LOW : GPIO_HIGH)));
    lastFlickerTime = SYS_TICK;
  }
}

void delay (uint32_t delay) {
  uint64_t start_time = SYS_TICK;
  while (SYS_TICK < start_time + delay);
}