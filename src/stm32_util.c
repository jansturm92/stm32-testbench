#include "stm32_util.h"
#include <string.h>

int _write(int file, char *ptr, int len);

int _write(int file, char *ptr, int len) {

    int i;
    (void)file;
    usart_wait_send_ready(STM32_USART);
    for (i = 0; i < len; i++) {
        if (ptr[i] == '\n') {
            usart_send_blocking(STM32_USART, '\r');
        }
        usart_send_blocking(STM32_USART, ptr[i]);
    }
    return i;
}

static void clock_setup(const clock_mode clock) {

#ifdef STM32F4DISCOVERY
    /* 24 MHz */
    const struct rcc_clock_scale benchmarkclock = {
        .pllm = 8,   // VCOin = HSE / PLLM = 1 MHz
        .plln = 192, // VCOout = VCOin * PLLN = 192 MHz
        .pllp = 8,   // PLLCLK = VCOout / PLLP = 24 MHz (low to have 0WS)
        .pllq = 4,   // PLL48CLK = VCOout / PLLQ = 48 MHz (required for USB, RNG)
        .pllr = 0,
        .hpre = RCC_CFGR_HPRE_DIV_NONE,
        .ppre1 = RCC_CFGR_PPRE_DIV_2,
        .ppre2 = RCC_CFGR_PPRE_DIV_NONE,
        .pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
        .voltage_scale = PWR_SCALE1,
        .flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_0WS,
        .ahb_frequency = 24000000,
        .apb1_frequency = 12000000,
        .apb2_frequency = 24000000,
    };
    switch (clock) {
    case CLOCK_BENCHMARK:
        rcc_clock_setup_pll(&benchmarkclock);
        break;
    case CLOCK_FAST:
    default:
        rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    }

    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_RNG);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART2);

    flash_prefetch_enable();
#endif
#ifdef STM32F0DISCOVERY
    switch (clock) {
    case CLOCK_BENCHMARK: /* 8MHz */
        rcc_osc_on(RCC_HSI);
        rcc_wait_for_osc_ready(RCC_HSI);
        rcc_set_sysclk_source(RCC_HSI);

        rcc_set_hpre(RCC_CFGR_HPRE_DIV2);
        rcc_set_ppre(RCC_CFGR_PPRE_NODIV);

        flash_prefetch_enable();
        flash_set_ws(FLASH_ACR_LATENCY_0WS);

        rcc_set_pll_multiplication_factor(RCC_CFGR_PLLMUL_MUL4);
        rcc_set_pll_source(RCC_CFGR_PLLSRC_HSI_CLK_DIV2);

        rcc_osc_on(RCC_PLL);
        rcc_wait_for_osc_ready(RCC_PLL);
        rcc_set_sysclk_source(RCC_PLL);

        rcc_apb1_frequency = 8000000;
        rcc_ahb_frequency = 8000000;
        break;
    case CLOCK_FAST:
    default:
        rcc_clock_setup_in_hsi_out_48mhz();
    }

    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART2);
#endif
#ifdef STM32L4DISCOVERY_IOT01A
    rcc_osc_on(RCC_HSI16);
    rcc_osc_on(RCC_MSI);
    rcc_set_msi_range(RCC_CR_MSIRANGE_24MHZ);

    flash_prefetch_enable();
    flash_dcache_enable();
    flash_icache_enable();
    rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, 4, 40, RCC_PLLCFGR_PLLP_DIV7,
                     RCC_PLLCFGR_PLLQ_DIV2, RCC_PLLCFGR_PLLR_DIV2);
    rcc_osc_on(RCC_PLL);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_RNG);

    /* choose MSI for RNG clock, 48 MHz not working for RNG,why? */
    rcc_set_clock48_source(RCC_CCIPR_CLK48SEL_MSI);
    switch (clock) {
    case CLOCK_BENCHMARK:
        flash_set_ws(FLASH_ACR_LATENCY_0WS);
        rcc_set_sysclk_source(RCC_CFGR_SW_MSI); // 24 MHz
        rcc_wait_for_sysclk_status(RCC_MSI);
        rcc_ahb_frequency = 24e6;
        rcc_apb1_frequency = 24e6;
        rcc_apb2_frequency = 24e6;
        break;
    case CLOCK_FAST:
    default:
        flash_set_ws(FLASH_ACR_LATENCY_4WS);
        rcc_set_sysclk_source(RCC_CFGR_SW_PLL); // 80 MHz
        rcc_wait_for_sysclk_status(RCC_PLL);
        rcc_ahb_frequency = 80e6;
        rcc_apb1_frequency = 80e6;
        rcc_apb2_frequency = 80e6;
    }

#endif
}

static void gpio_setup(void) {

    gpio_mode_setup(STM32_TXRX_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, STM32_TXRX_PIN);
    gpio_set_af(STM32_TXRX_GPIO_PORT, STM32_TXRX_GPIO_AF, STM32_TXRX_PIN);
    gpio_mode_setup(STM32_LEDS_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    STM32_LEDS_PIN);
}

void usart_setup(void) {

    usart_set_baudrate(STM32_USART, STM32_USART_BAUDRATE);
    usart_set_databits(STM32_USART, 8);
    usart_set_stopbits(STM32_USART, USART_STOPBITS_1);
    usart_set_mode(STM32_USART, USART_MODE_TX_RX);
    usart_set_parity(STM32_USART, USART_PARITY_NONE);
    usart_set_flow_control(STM32_USART, USART_FLOWCONTROL_NONE);
    usart_enable(STM32_USART);
}

static void systick_setup(void) {

    systick_set_reload(STK_RELOAD);
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();
    systick_interrupt_enable();
}

volatile uint32_t sys_count;
volatile uint32_t overflow_count;

void sys_tick_handler(void) { sys_count++; }

void sleep(uint32_t delay) {

    uint32_t wake = sys_count + delay;
    while (wake > sys_count)
        ;
}

void stm32_leds_toggle(void) { gpio_toggle(STM32_LEDS_GPIO_PORT, STM32_LEDS_PIN); }

void print_cyc(const char *s, uint32_t cyc) {

    printf("\t--> %s: %lu cyc\n", s, (overflow_count + 1) * STK_RELOAD - cyc);
}

void print_hex(const char *s, const unsigned char *c, int len) {

    int i;
    printf("\n%s= ", s);
    for (i = 0; i < len; i++) {
        printf("%x", c[i]);
    }
    printf("\n");
}

__attribute__((weak)) void randombytes(unsigned char *x, size_t xlen) {
#ifdef RNG
    union {
        unsigned char aschar[4];
        uint32_t asint;
    } random;

    while (xlen > 4) {
        random.asint = rng_get_random_blocking();
        *x++ = random.aschar[0];
        *x++ = random.aschar[1];
        *x++ = random.aschar[2];
        *x++ = random.aschar[3];
        xlen -= 4;
    }
    if (xlen > 0) {
        for (random.asint = rng_get_random_blocking(); xlen > 0; --xlen) {
            *x++ = random.aschar[xlen - 1];
        }
    }

#else
#pragma message("No TRNG available on this board")
    memset(x, 0xab, xlen);
#endif
}

void stm32_print_boardinfo(void) {

    printf("FRAMEWORK\t= %s\n", PIO_FRAMEWORK);
    printf("BOARD\t\t= %s\n", PIO_BOARD);
    printf("  > UID\t\t= 0x%lx-0x%lx-0x%lx\n", DESIG_UNIQUE_ID2, DESIG_UNIQUE_ID1, DESIG_UNIQUE_ID0);
}

void stm32_setup(const clock_mode clock) {

    clock_setup(clock);
    gpio_setup();
    usart_setup();
    systick_setup();
#ifdef RNG
    rng_enable();
#endif
}
