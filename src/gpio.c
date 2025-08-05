#include "global.h"

int32_t GPIO_Init(void) {
    DDRD = 0x00;
    DDRB = 0x00;
    PORTD = 0x00;
    PORTB = 0x00;
    return GPIO_SUCCESS;
}

int32_t GPIO_Mode(uint8_t pin, uint8_t direction) {
    if(pin > GPIO_MAX) return GPIO_ERROR_RANGE;
    if(direction > GPIO_OUTPUT) return GPIO_ERROR_RANGE;

    if (direction) {    //output
        if(pin < 8) DDRD |= (1 << pin);
        else DDRB |= (1 << (pin & 7));
    } else {            //input
        if(pin < 8) DDRD &= ~(1 << pin);
        else DDRB &= ~(1 << (pin & 7));
    }
    return GPIO_SUCCESS;
}

int32_t GPIO_Write(uint8_t pin, uint8_t state) {
    if (pin > GPIO_MAX) return GPIO_ERROR_RANGE;
    if (state > GPIO_HIGH) return GPIO_ERROR_RANGE;

    if (state) {        //high
        if(pin < 8) PORTD |= (1 << pin);
        else PORTB |= (1 << (pin & 7));
    } else {            //low
        if(pin < 8) PORTD &= ~(1 << pin);
        else PORTB &= ~(1 << (pin & 7));
    }
    return GPIO_SUCCESS;
}

int32_t GPIO_Read(uint8_t pin) {
    if (pin > GPIO_MAX) return GPIO_ERROR_RANGE;

    int32_t state;
    if(pin < 8) state = PIND & (1 << pin);
    else state = PINB & (1 << (pin & 7));
    
    return state;
}

int32_t GPIO_Toggle(uint8_t pin) {
    if (pin > GPIO_MAX) return GPIO_ERROR_RANGE;   
    
    if(pin < 8) PORTD ^= (1 << pin);
    else PORTB ^= (1 << (pin & 7));

    return SYS_SUCCESS;
}