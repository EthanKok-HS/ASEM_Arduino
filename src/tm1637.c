#include "global.h"

void _delay(void);
uint8_t _decode(uint8_t digit);

int32_t TM1637_Init(void) {
    return TM1637_SUCCESS;
}

int32_t TM1637_Reset() {
    SYS_Error_Check(I2C_Start());
    SYS_Error_Check(I2C_Address_Write(0x40));
    SYS_Error_Check(I2C_Stop());

    _delay();

    SYS_Error_Check(I2C_Start());
    SYS_Error_Check(I2C_Address_Write(0xC0));
    for (uint16_t i = 0; i < 6; i++) {
        I2C_Data_Send(0x00);
    }
    SYS_Error_Check(I2C_Stop());

    _delay();

    SYS_Error_Check(I2C_Start());
    I2C_Address_Read(0x8E);
    SYS_Error_Check(I2C_Stop());

    _delay();

    return TM1637_SUCCESS;
}

int32_t TM1637_Write(int16_t value) {
    SYS_Error_Check(I2C_Start());
    SYS_Error_Check(I2C_Address_Write(0x44));
    SYS_Error_Check(I2C_Stop());

    _delay();

    SYS_Error_Check(I2C_Start());
    SYS_Error_Check(I2C_Address_Write(0xC0));
    if (value > 9999 || value < -999) {
        for (uint16_t i = 0; i < 6; i++) {
            I2C_Data_Send(DISP_DASH);
        }
    }
    else if(value >= 0) {
        I2C_Data_Send(_decode(value % 10));
        I2C_Data_Send(_decode((value % 100) / 10));
        I2C_Data_Send(_decode((value % 1000) / 100));
        I2C_Data_Send(_decode(value / 1000));
        I2C_Data_Send(0x00);
        I2C_Data_Send(0x00);
    } else {
        I2C_Data_Send(_decode(value % 10));
        I2C_Data_Send(_decode((value % 100)) / 10);
        I2C_Data_Send(_decode((value % 1000) / 100));
        I2C_Data_Send(DISP_DASH);
        I2C_Data_Send(0x00);
        I2C_Data_Send(0x00);
    }
    SYS_Error_Check(I2C_Stop());

    _delay();

    SYS_Error_Check(I2C_Start());
    I2C_Address_Read(0x8E);
    SYS_Error_Check(I2C_Stop());

    _delay();

    return TM1637_SUCCESS;
}

void _delay() {
    uint64_t timeout = SYS_TICK + 2;
    while(SYS_TICK < timeout);
}

uint8_t _decode(uint8_t digit) {
    switch (digit) {
        case 0: return DISP_0;
        case 1: return DISP_1;
        case 2: return DISP_2;
        case 3: return DISP_3;
        case 4: return DISP_4;
        case 5: return DISP_5;
        case 6: return DISP_6;
        case 7: return DISP_7;
        case 8: return DISP_8;
        case 9: return DISP_9;
        default: return DISP_DASH;
    }
}
