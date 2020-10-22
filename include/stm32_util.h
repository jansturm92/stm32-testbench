#ifndef STM32_UTIL_H
#define STM32_UTIL_H

#define PIO_FRAMEWORK "libopencm3"
#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <stdint.h>
#include <stdio.h>

#ifdef STM32F4DISCOVERY
#define PIO_BOARD "disco_f407vg"
#include <libopencm3/stm32/rng.h>
#include <libopencm3/stm32/f4/nvic.h>
#define RNG
#define STM32_USART USART2
#define STM32_USART_BAUDRATE 115200
#define STM32_UNITTEST_INITIAL_SLEEP_DURATION 40
#define STM32_TXRX_GPIO_PORT GPIOA
#define STM32_TXRX_GPIO_AF GPIO_AF7
#define STM32_TXRX_PIN (GPIO2 | GPIO3)
#define STM32_LEDS_GPIO_PORT GPIOD
#define STM32_LEDS_PIN (GPIO12 | GPIO13 | GPIO14 | GPIO15)
#endif

#ifdef STM32L4DISCOVERY_IOT01A
#define PIO_BOARD "disco_l475vg_iot01a"
#include <libopencm3/stm32/rng.h>
#include <libopencm3/stm32/l4/nvic.h>
#define RNG
#define STM32_USART USART1
#define STM32_USART_BAUDRATE 115200
#define STM32_UNITTEST_INITIAL_SLEEP_DURATION 10
#define STM32_TXRX_GPIO_PORT GPIOB
#define STM32_TXRX_GPIO_AF GPIO_AF7
#define STM32_TXRX_PIN (GPIO6 | GPIO7)
#define STM32_LEDS_GPIO_PORT GPIOB
#define STM32_LEDS_PIN (GPIO14)
#endif

#ifdef STM32F0DISCOVERY
#define PIO_BOARD "disco_f051r8"
#include <libopencm3/stm32/f0/nvic.h>
#define STM32_USART USART2
#define STM32_USART_BAUDRATE 38400
#define STM32_UNITTEST_INITIAL_SLEEP_DURATION 1
#define STM32_TXRX_GPIO_PORT GPIOA
#define STM32_TXRX_GPIO_AF GPIO_AF1
#define STM32_TXRX_PIN (GPIO2 | GPIO3)
#define STM32_LEDS_GPIO_PORT GPIOC
#define STM32_LEDS_PIN (GPIO8 | GPIO9)
#endif

#define STK_RELOAD 0x00FFFFFF
extern volatile uint32_t overflow_count;
extern volatile uint32_t sys_count;

#define MEASURE(name, func)                                                              \
    overflow_count = sys_count;                                                          \
    STK_CVR = 0;                                                                         \
    func;                                                                                \
    cyc = STK_CVR;                                                                       \
    overflow_count = sys_count - overflow_count;                                         \
    print_cyc(name, cyc);

typedef enum { CLOCK_BENCHMARK, CLOCK_FAST } clock_mode;

void sleep(uint32_t delay);
void stm32_leds_toggle(void);
void randombytes(unsigned char *x, size_t xlen);
void usart_setup(void);
void stm32_setup(const clock_mode clock);
void stm32_print_boardinfo(void);

void print_cyc(const char *s, uint32_t cyc);
void print_hex(const char *s, const unsigned char *c, int len);

#endif // STM32_UTIL_H
