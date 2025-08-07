#pragma once

#define TM1637_ERROR_BASE 0x00006000
#define TM1637_SUCCESS 0
#define TM1637_ERROR_RANGE (TM1637_ERROR_BASE + 1)

#define DIGIT_0 0xC0
#define DIGIT_1 0xC4
#define DIGIT_2 0xC1
#define DIGIT_3 0xC3

#define DISP_COLON  0x01
#define DISP_DASH   0x02

#define DISP_0 0b11111100
#define DISP_1 0b00001100
#define DISP_2 0b11011010
#define DISP_3 0b10011110
#define DISP_4 0b00101110
#define DISP_5 0b10110110
#define DISP_6 0b11110110
#define DISP_7 0b00011100
#define DISP_8 0b11111110
#define DISP_9 0b10111110

extern int32_t TM1637_Init(void);
extern int32_t TM1637_Reset(void);
extern int32_t TM1637_Write(int16_t value);