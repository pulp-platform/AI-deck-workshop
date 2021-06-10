#ifndef __UART_DMA_SETUP_H
#define __UART_DMA_SETUP_H

#include "uart_dma_pulp.h"

void USART_DMA_Start(uint32_t baudrate, uint8_t *pulpRxBuffer, uint32_t BUFFERSIZE);

#endif
