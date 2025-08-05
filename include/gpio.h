#pragma once

#define GPIO_MIN    0
#define GPIO_MAX    13

#define GPIO_ERROR_BASE     0x00003000
#define GPIO_SUCCESS        0
#define GPIO_ERROR_RANGE    (GPIO_ERROR_BASE + 1)
#define GPIO_ERROR_VALUE    (GPIO_ERROR_BASE + 2)

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

#define GPIO_LOW    0
#define GPIO_HIGH   1

extern int32_t GPIO_Init(void);
extern int32_t GPIO_Mode(uint8_t pin, uint8_t direction);
extern int32_t GPIO_Write(uint8_t pin, uint8_t state);
extern int32_t GPIO_Read(uint8_t pin);
extern int32_t GPIO_Toggle(uint8_t pin);