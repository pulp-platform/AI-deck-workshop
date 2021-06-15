/*-----------------------------------------------------------------------------
 Copyright (C) 2020-2021 ETH Zurich, Switzerland, University of Bologna, Italy.
 All rights reserved.                                                           
                                                                                                           
 File:    uart_dma_setup.h   
 Author:  Vlad Niculescu      <vladn@iis.ee.ethz.ch>                           
 Date:    15.03.2021                                                           
-------------------------------------------------------------------------------*/

#ifndef __UART_DMA_SETUP_H
#define __UART_DMA_SETUP_H

#include "uart_dma_pulp.h"

void USART_DMA_Start(uint32_t baudrate, uint8_t *pulpRxBuffer, uint32_t BUFFERSIZE);

#endif
