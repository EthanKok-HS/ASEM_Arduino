#pragma once

#define SYS_SUCCESS 0
#define SYS_ERROR   1

#define LED_ERROR_PIN 13
#define SYS_Error_Check(x) if((SYS_ERROR_NUM = (x))) SYS_fatal_error(SYS_ERROR_NUM) 

#define SYS_DEBOUNCE_DELAY 150

volatile uint64_t SYS_TICK;
int32_t SYS_ERROR_NUM;

extern void SYS_fatal_error(int32_t err);
extern int32_t SYS_Init(void);