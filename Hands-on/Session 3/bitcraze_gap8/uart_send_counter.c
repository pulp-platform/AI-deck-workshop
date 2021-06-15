/*-----------------------------------------------------------------------------
 Copyright (C) 2020-2021 ETH Zurich, Switzerland, University of Bologna, Italy.
 All rights reserved.                                                                                                      
                                                                               
 File:    uart_send_counter.c   
 Author:  Vlad Niculescu      <vladn@iis.ee.ethz.ch>                           
 Date:    15.03.2021                                                           
-------------------------------------------------------------------------------*/

/* PMSIS includes */
#include "pmsis.h"

/* Variables used. */
uint8_t to_send;

void test_uart_helloworld(void)
{
    printf("Entering main controller\n");

    uint32_t errors = 0;
    struct pi_device uart;
    struct pi_uart_conf conf;

    /* Init & open uart. */
    pi_uart_conf_init(&conf);
    conf.enable_tx = 1;
    conf.enable_rx = 0;
    conf.baudrate_bps = 115200;
    pi_open_from_conf(&uart, &conf);
    if (pi_uart_open(&uart))
    {
        printf("Uart open failed !\n");
        pmsis_exit(-1);
    }

    for (uint8_t i=0; i<100; i++)
    {
        to_send = i;
        pi_uart_write(&uart, &to_send, 1);
        pi_time_wait_us(500000);
    }

    pi_uart_close(&uart);

    pmsis_exit(errors);
}

/* Program Entry. */
int main(void)
{
    printf("\n\n\t *** PMSIS Uart HelloWorld ***\n\n");
    return pmsis_kickoff((void *) test_uart_helloworld);
}
