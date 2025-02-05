/* USER CODE BEGIN Header */
/**

/* Includes ------------------------------------------------------------------*/
#include "drv_delay.h"
#include "main.h"


//static uint32_t fac_us;
static uint32_t fac_ms;

void delay_init(void)
{
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 8000000);

#if SYSTEM_SUPPORT_RTOS
    uint32_t reload;
    reload = SystemCoreClock / 8000000;
    reload *= 1000000 / SystemCoreClock;
    fac_ms = 1000 / SystemCoreClock;

    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SysTick_IRQn);
#else
    fac_ms = (uint16_t)fac_us * 1000;
#endif
}

void delay_us(uint32_t us)
{
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() - start) < us / 1000);
}

void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

