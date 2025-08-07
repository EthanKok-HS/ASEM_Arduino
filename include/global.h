#pragma once

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#if __has_include("timer2.h")
#include "timer2.h"
#else 
#include "timer0.h"
#endif

#include "sys.h"
#include "gpio.h"
#include "uart.h"
#include "adc.h"
#include "i2c.h"
#include "tm1637.h"

#define LED_DEBUG_PIN 13