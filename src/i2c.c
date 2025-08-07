#include "global.h"

int32_t I2C_Init(void) {
    TWSR = 0x01;
    TWBR = 18;
}

int32_t I2C_Start(void) {
    TWCR = 0xA4;
    uint64_t timeout = SYS_TICK + I2C_TIMEOUT_DURATION;
    while ((TWCR & 0x80) == 0x00 && SYS_TICK < timeout);
    
    if(SYS_TICK >= timeout) return I2C_ERROR_TIMEOUT;
    if ((TWSR & 0xF8) == 0x08) return I2C_SUCCESS;
    else return I2C_ERROR_START;
}

int32_t I2C_Restart(void) {
    TWCR = 0xA4;
    uint64_t timeout = SYS_TICK + I2C_TIMEOUT_DURATION;
    while ((TWCR & 0x80) == 0x00 && SYS_TICK < timeout);
    if(SYS_TICK >= timeout) return I2C_ERROR_TIMEOUT;

    if ((TWSR & 0xF8) == 0x08) return I2C_SUCCESS;
    else return I2C_ERROR_START;
}

int32_t I2C_Address_Write(uint8_t add) {
    TWDR = (add << 1) & 0xFE;
    TWCR = 0x84;
    uint64_t timeout = SYS_TICK + I2C_TIMEOUT_DURATION;
    while ((TWCR & 0x80) == 0x00 && SYS_TICK < timeout);

    if(SYS_TICK >= timeout) return I2C_ERROR_TIMEOUT;
    if ((TWSR & 0xF8) == 0x18) return I2C_SUCCESS;
    else return I2C_ERROR_ADDR_W; 
}
 
int32_t I2C_Address_Read(uint8_t add) {
    TWDR = (add << 1) | 0x01;
    TWCR = 0x84;
    uint64_t timeout = SYS_TICK + I2C_TIMEOUT_DURATION;
    while ((TWCR & 0x80) == 0x00 && SYS_TICK < timeout);

    if(SYS_TICK >= timeout) return I2C_ERROR_TIMEOUT;
    if ((TWSR & 0xF8) == 0x18) return I2C_SUCCESS;
    else return I2C_ERROR_ADDR_R; 
}
int32_t I2C_Stop(void) {
    TWCR = 0x94;
    uint64_t timeout = SYS_TICK + I2C_TIMEOUT_DURATION;
    while ((TWCR & 0x10) == 0x10 && SYS_TICK < timeout);
    if(SYS_TICK >= timeout) return I2C_ERROR_TIMEOUT;
    return I2C_SUCCESS;
}

int32_t I2C_Data_Send(uint8_t byte) {
    TWDR = byte;
    TWCR = 0x84;
    uint64_t timeout = SYS_TICK + I2C_TIMEOUT_DURATION;
    while((TWCR & 0x80) == 0x00 && SYS_TICK < timeout);

    if(SYS_TICK >= timeout) return I2C_ERROR_TIMEOUT;
    if((TWSR & 0xF8) == 0x28) return I2C_SUCCESS;
    else if((TWSR & 0xF8) == 0x30) return I2C_ERROR_DATA_SENT_NO_ACK;
    else return I2C_ERROR_DATA_SENT_ACK;
}

int32_t I2C_Data_Read(uint8_t* data) {
    TWCR = 0x84;
    uint64_t timeout = SYS_TICK + I2C_TIMEOUT_DURATION;
    while((TWCR & 0x80) == 0x00 && SYS_TICK < timeout);
    *data = TWDR;

    if(SYS_TICK >= timeout) return I2C_ERROR_TIMEOUT;
    if((TWSR & 0xF8) == 0x58) return I2C_SUCCESS;
    else return I2C_ERROR_DATA_READ_NO_ACK;
}

int32_t I2C_Data_Read_ACK(uint8_t* data) {
    TWCR = 0xC4;
    uint64_t timeout = SYS_TICK + I2C_TIMEOUT_DURATION;
    while((TWCR & 0x80) == 0x00 && SYS_TICK < timeout);
    *data = TWDR;

    if(SYS_TICK >= timeout) return I2C_ERROR_TIMEOUT;
    if((TWSR & 0xF8) == 0x50) return I2C_SUCCESS;
    else return I2C_ERROR_DATA_READ_NO_ACK;
}