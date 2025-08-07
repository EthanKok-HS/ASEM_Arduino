#pragma once

#define TIMER0_ERROR_BASE 0x00004000
#define TIMER_SUCCESS 0
#define TIMER0_ERROR_RANGE (TIMER0_ERROR_BASE + 1)

extern int32_t TIMER0_Init(void);