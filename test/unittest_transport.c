#include "unittest_transport.h"
#include "stm32_util.h"
#include <libopencm3/stm32/usart.h>

void unittest_uart_begin(void) { usart_setup(); }

void unittest_uart_putchar(char c) {

    usart_wait_send_ready(STM32_USART);
    if (c == '\n') {
        usart_send_blocking(STM32_USART, '\r');
    }
    usart_send_blocking(STM32_USART, c);
}

void unittest_uart_flush(void) {}

void unittest_uart_end(void) {}