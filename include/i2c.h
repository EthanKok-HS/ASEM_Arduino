#pragma once

#define I2C_ERROR_BASE  0x00007000
#define I2C_SUCCESS     0
#define I2C_ERROR_START             (I2C_ERROR_BASE + 1)
#define I2C_ERROR_ADDR_W            (I2C_ERROR_BASE + 2)
#define I2C_ERROR_ADDR_R            (I2C_ERROR_BASE + 3)
#define I2C_ERROR_DATA_SENT_ACK     (I2C_ERROR_BASE + 4)
#define I2C_ERROR_DATA_SENT_NO_ACK  (I2C_ERROR_BASE + 5)
#define I2C_ERROR_DATA_READ_ACK     (I2C_ERROR_BASE + 6)
#define I2C_ERROR_DATA_READ_NO_ACK  (I2C_ERROR_BASE + 7)
#define I2C_ERROR_TIMEOUT           (I2C_ERROR_BASE + 9)

#define I2C_TIMEOUT_DURATION    10

extern int32_t I2C_Init(void);
extern int32_t I2C_Start(void);
extern int32_t I2C_Restart(void);
extern int32_t I2C_Address_Write(uint8_t add);
extern int32_t I2C_Address_Read(uint8_t add);
extern int32_t I2C_Stop(void);
extern int32_t I2C_Data_Send(uint8_t byte);
extern int32_t I2C_Data_Read(uint8_t* data);
extern int32_t I2C_Data_Read_ACK(uint8_t* data);
