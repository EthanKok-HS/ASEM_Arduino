#pragma once

#define TIMER2_ERROR_BASE 0x00001100
#define TIMER2_SUCCESS 0
#define TIMER2_ERROR_RANGE (TIMER2_ERROR_BASE + 1)

extern int32_t TIMER2_Init(void);