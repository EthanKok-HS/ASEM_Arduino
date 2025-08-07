// I2C 7-segment display datasheet: 
// https://www.puntoflotante.net/DATASHEET-TM1637.pdf

#include "global.h"

#define FLICKER_DELAY 1000
#define READ_DELAY    50
#define ANALOG_DELAY  100
#define COUNTER_DELAY 50

#define READ_PIN        5
#define READ_PIN_2      6
#define TOGGLE_LED_PIN  7
#define MAX_LED_COUNT   4

#define DATASET_SIZE    8

#define COUNTER_UP_PIN    10
#define COUNTER_RESET_PIN 11

void heartbeat(void);
void serial_feedback_loop(void);
void analog_read_loop(void);

void delay(uint32_t delay);
void led_wave(uint32_t wave_count, uint32_t delay, uint8_t rebound);
void push_back (uint16_t *dataset, uint16_t data);
int32_t data_average(const uint16_t* dataset);
void i2c_send_byte(uint8_t addr, uint8_t data);

volatile uint64_t lastFlickerTime = 0;
volatile uint64_t lastReadTime = 0;
volatile uint64_t lastAnalogTime = 0;
volatile uint64_t lastCounterTime = 0;

uint16_t ana0[DATASET_SIZE];

int main(void) {
  SYS_Error_Check(SYS_Init());

  uint8_t lastState1, lastState2, lastState3, lastState4;
  uint32_t count = 0;
  uint32_t led_count = 0; 

  uint16_t counter = 0;
  TM1637_Write(counter);

  for (;;) {
    heartbeat();
    uartProcess(); 

    serial_feedback_loop();
    analog_read_loop();

    // if (SYS_TICK - lastCounterTime > COUNTER_DELAY) {
    //   uint8_t currentState = GPIO_Read(COUNTER_UP_PIN);
    //   if (currentState != lastState3) {
    //     if (currentState) {
    //       counter++;
    //       TM1637_Write(counter);
    //     }
    //     lastState3 = currentState;
    //   }

    //   currentState = GPIO_Read(COUNTER_RESET_PIN);
    //   if (currentState != lastState4) {
    //     if (currentState) {
    //       counter = 0;
    //       TM1637_Write(counter);
    //     }
    //     lastState4 = currentState;
    //   }
    //   lastCounterTime = SYS_TICK;
    // }

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

void i2c_send_byte(uint8_t addr, uint8_t data) {
  SYS_Error_Check(I2C_Start());
  SYS_Error_Check(I2C_Address_Write(0xA0));
  SYS_Error_Check(I2C_Data_Send(addr));
  SYS_Error_Check(I2C_Data_Send(data));
  SYS_Error_Check(I2C_Stop());
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

void serial_feedback_loop(void) {
  uint8_t feedback;
  if (uartRead(&feedback) == 0) {
    uartPrint(&feedback);
  }
}

void analog_read_loop(void) {
  if (SYS_TICK - lastAnalogTime > ANALOG_DELAY) {
    push_back(ana0, ADC_Read_Single(0));

    uartPrint("\nAnalog 0: ");
    uartPrintInt32(data_average(ana0));
    TM1637_Write(data_average(ana0));

    lastAnalogTime = SYS_TICK;
  }
}

void delay(uint32_t delay) {
  uint64_t start_time = SYS_TICK;
  while (SYS_TICK < start_time + delay);
}

void push_back(uint16_t *dataset, uint16_t data) {
  uint32_t data_size = sizeof(dataset) / sizeof(dataset[0]);
  for (int32_t i = data_size - 1;  i >= 1; i--) {
    dataset[i] = dataset[i - 1];
  }
  dataset[0] = data;
}

int32_t data_average(const uint16_t* dataset) {
  uint32_t total;
  uint32_t data_size = sizeof(dataset) / sizeof(dataset[0]);
  for(int32_t i = 0; i < data_size; i++) {
    total += dataset[i];
  }
  return total / data_size;
}