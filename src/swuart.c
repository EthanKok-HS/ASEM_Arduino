#include "global.h"
uint8_t SWUART_TX_BUF[SWUART_TX_BUF_SIZE];
uint8_t SWUART_TX_BUF_WR, SWUART_TX_BUF_RD, SWUART_TX_BYTES;
uint8_t SWUART_RX_BUF[SWUART_RX_BUF_SIZE];
uint8_t SWUART_RX_BUF_WR, SWUART_RX_BUF_RD, SWUART_RX_BYTES;

typedef enum {
	STATE_IDLE,
	STATE_START,
	STATE_DATA,
	STATE_STOP
	} SWUART_STATE_e;

SWUART_STATE_e TX_STATE;
SWUART_STATE_e RX_STATE;

int32_t SWUART_Init(void) {
	SWUART_TX_BUF_WR = 0; SWUART_TX_BUF_RD = 0; SWUART_TX_BYTES = 0;
	SWUART_RX_BUF_WR = 0; SWUART_RX_BUF_RD = 0; SWUART_RX_BYTES = 0;
	TX_STATE = STATE_IDLE; RX_STATE = STATE_IDLE;

	GPIO_Write(SWUART_TX_PIN, GPIO_HIGH);
	GPIO_Mode(SWUART_TX_PIN, GPIO_OUTPUT);
	GPIO_Mode(SWUART_RX_PIN, GPIO_INPUT);

	return SWUART_SUCCESS;
	}

int32_t SWUART_Write(uint8_t byte) {
	if(SWUART_TX_BYTES >= SWUART_TX_BUF_WR) {			//buffer full
		while(SWUART_TX_BYTES >= SWUART_TX_BUF_SIZE);
		}

	SWUART_TX_BUF[SWUART_TX_BUF_WR] = byte;
	SWUART_TX_BUF_WR = (SWUART_TX_BUF_WR + 1) & (SWUART_TX_BUF_SIZE - 1);
	SWUART_TX_BYTES++;

	if(TX_STATE == STATE_IDLE) TX_STATE = STATE_START;

	return SWUART_SUCCESS;
	}

int32_t SWUART_Write_String(uint8_t *str, uint8_t size) {
	uint8_t i;
	
	for(i = 0; i < size; i++) {
		if(SWUART_TX_BYTES >= SWUART_TX_BUF_WR) {			//buffer full
			while(SWUART_TX_BYTES >= SWUART_TX_BUF_SIZE) SWUART_Process();		
			}

		SWUART_TX_BUF[SWUART_TX_BUF_WR] = *str;
		str++;
		SWUART_TX_BUF_WR = (SWUART_TX_BUF_WR + 1) & (SWUART_TX_BUF_SIZE - 1);
		SWUART_TX_BYTES++;
		}

	if(TX_STATE == STATE_IDLE) TX_STATE = STATE_START;

	return SWUART_SUCCESS;
	}

int32_t SWUART_Read(uint8_t *byte) {
	if(SWUART_RX_BYTES == 0) return SWUART_ERROR_RX_BUF_EMPTY;
	
	*byte = SWUART_RX_BUF[SWUART_RX_BUF_RD];			//read buffer
	SWUART_RX_BUF_RD = (SWUART_RX_BUF_RD + 1) & (SWUART_RX_BUF_SIZE - 1);
	SWUART_RX_BYTES--;

	return SWUART_SUCCESS;
	}

//must be called in baudrate * 4 ISR
int32_t SWUART_Process(void) {
	static uint8_t tx_byte, tx_count;
	static uint8_t rx_byte, rx_count;
	
	switch(TX_STATE) {
		case STATE_IDLE: {
			if(SWUART_TX_BYTES > 0)	{
				tx_byte = SWUART_TX_BUF[SWUART_TX_BUF_RD];
				SWUART_TX_BUF_RD = (SWUART_TX_BUF_RD + 1) & (SWUART_TX_BUF_SIZE - 1);
				SWUART_TX_BYTES--;
	
				tx_count = 0;
				TX_STATE = STATE_START;
				}
			break;
			}
		case STATE_START: {
			if(tx_count == 3) TX_STATE = STATE_DATA;

			tx_count++;
			break;
			}
		case STATE_DATA: {
			if(tx_count == 35) TX_STATE = STATE_STOP;
			else if((tx_count & 3) == 0) {
				GPIO_Write(SWUART_TX_PIN, (tx_byte & 0x01));
				tx_byte >>= 1;
				}

			tx_count++;
			break;
			}
		case STATE_STOP: {
			if(tx_count == 39) TX_STATE = STATE_IDLE; 
			else if(tx_count == 36) GPIO_Write(SWUART_TX_PIN, GPIO_HIGH);

			tx_count++;
			break;
			}
		}
	
	switch (RX_STATE) {
		case STATE_IDLE: {
			if (!GPIO_Read(SWUART_RX_PIN)) {
				rx_count = 1;
				rx_byte = 0;
				RX_STATE = STATE_START;
			}
			break;
		}

		case STATE_START: {
			if (rx_count == 2) RX_STATE = STATE_DATA;
			if (GPIO_Read(SWUART_RX_PIN)) RX_STATE = STATE_IDLE;
			rx_count++;
			break;
		}

		case STATE_DATA: {
			if(rx_count == 35) RX_STATE = STATE_STOP;
			else if((rx_count & 3) == 2) {
					rx_byte >>= 1; 	
					if(GPIO_Read(SWUART_RX_PIN)) rx_byte |= 0x80;
				}
			rx_count++;
			break;
		}
		
		case STATE_STOP: {
			if (rx_count == 38) {
				if (GPIO_Read(SWUART_RX_PIN)) {
					if (SWUART_RX_BYTES < SWUART_RX_BUF_SIZE) {
						SWUART_RX_BUF[SWUART_RX_BUF_WR] = rx_byte;
						SWUART_RX_BUF_WR = (SWUART_RX_BUF_WR + 1) & (SWUART_RX_BUF_SIZE - 1);
						SWUART_RX_BYTES++;
					}	
				}
			}
			else if (rx_count == 39) RX_STATE = STATE_IDLE;
			rx_count++;
			break;
		}
	}

	return SWUART_SUCCESS;
	}
