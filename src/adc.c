#include "global.h"

int32_t ADC_Init(void){
    ADMUX = 0x40;
    ADCSRA = 0x84;

    return ADC_SUCCESS;
}

uint16_t ADC_Read_Single(uint8_t chan) {
    uint16_t value;

    ADMUX &= ~0x0F;
    ADMUX |= (chan & 0x0F);

    ADCSRA |= 0x40;
    while ((ADCSRA & 0x40) == 0x40);
    value = ADCL;
    value = value + ((uint16_t) ADCH << 8);

    return value;
}