#include "FreeRTOS.h"
#include "usart.h"
#include "uart_task.h"
#include "queue.h"
#include "cmsis_os2.h"

static TaskHandle_t uartRxTaskHandle; 

static const osThreadAttr_t uartRxTask_attributes = {
    .name = "uart_rx_task",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 128 * 8};

#define RX_BUF_SIZE 256

#include <stdio.h>
void ProcessMessage(uint8_t *msg, uint32_t len)
{
    (void)len;
    printf("%s\n", msg);
}

static void UartRxTask(void *argument)
{
    (void)argument;
    uint8_t rxBuf[RX_BUF_SIZE];
    uint8_t ch;
    uint32_t idx = 0;

    for (;;)
    {
        // Block until a single byte is received (HAL_MAX_DELAY = no timeout)
        if (HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY) == HAL_OK)
        {
            // Accumulate until end of line
            if (ch == '\n' || ch == '\r')
            {
                if (idx > 0)
                {
                    rxBuf[idx] = 0;             // end of string
                    ProcessMessage(rxBuf, idx); // processing of the received string
                    idx = 0;
                }
            }
            else
            {
                if (idx < RX_BUF_SIZE - 1)
                    rxBuf[idx++] = ch;
                else
                    idx = 0; // on overflow -> reset
            }
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

void uart_task_init(void)
{
    uartRxTaskHandle = osThreadNew(UartRxTask, NULL, &uartRxTask_attributes);
}