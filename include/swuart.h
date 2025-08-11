#ifndef __SWUART_H__
#define __SWUART_H__
#ifdef __cplusplus
extern "C" {
#endif

#define SWUART_ERROR_BASE			0x00004100
#define SWUART_SUCCESS				0
#define SWUART_ERROR_TX_BUF_FULL	(SWUART_ERROR_BASE + 1)
#define SWUART_ERROR_RX_BUF_EMPTY	(SWUART_ERROR_BASE + 2) 

#define SWUART_TX_PIN				2
#define SWUART_RX_PIN				3
#define SWUART_TX_BUF_SIZE			32		//MUST BE 2^N size
#define SWUART_RX_BUF_SIZE			32		//MUST BE 2^N size

extern uint8_t SWUART_TX_BYTES;
extern uint8_t SWUART_RX_BYTES;
	
extern int32_t SWUART_Init(void);
extern int32_t SWUART_Write(uint8_t byte);
extern int32_t SWUART_Read(uint8_t *byte);
extern int32_t SWUART_Write_String(uint8_t *str, uint8_t size);
extern int32_t SWUART_Process(void);

#ifdef __cplusplus
}
#endif
#endif
