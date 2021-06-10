#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "app.h"
#include "FreeRTOS.h"
#include "task.h"
#include "debug.h"
#include "uart_dma_setup.h"
#include "log.h"

#define DEBUG_MODULE "HELLOWORLD"
#define BUFFERSIZE 1

uint8_t aideckRxBuffer[BUFFERSIZE];
volatile uint8_t dma_flag = 0;
uint8_t log_counter=0;

void appMain()
{
	DEBUG_PRINT("Application started! \n");
	USART_DMA_Start(115200, aideckRxBuffer, BUFFERSIZE);

	while(1) {
		vTaskDelay(M2T(100));
		if (dma_flag == 1)
		{
			dma_flag = 0;  // clear the flag
			DEBUG_PRINT("Counter: %d\n", aideckRxBuffer[0]);
			log_counter = aideckRxBuffer[0];
			memset(aideckRxBuffer, 0, BUFFERSIZE);  // clear the dma buffer
		}
	}
}


void __attribute__((used)) DMA1_Stream1_IRQHandler(void)
{
 DMA_ClearFlag(DMA1_Stream1, UART3_RX_DMA_ALL_FLAGS);
 dma_flag = 1;
}

LOG_GROUP_START(log_test)
LOG_ADD(LOG_UINT8, test_variable_x, &log_counter)
LOG_GROUP_STOP(log_test)