#pragma once

#define ADC_ERROR_BASE  0x00008000
#define ADC_SUCCESS     0
#define ADC_ERROR_RANGE (ADC_ERROR_BASE + 1)

extern int32_t ADC_Init(void);
extern uint16_t ADC_Read_Single(uint8_t chan); 