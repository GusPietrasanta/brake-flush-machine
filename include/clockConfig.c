#include "stm32f103x6.h"

void systemClockConfig(void)
{
	FLASH->ACR |= FLASH_ACR_LATENCY_2;
	//while (!(FLASH->ACR & FLASH_ACR_LATENCY_2));

	RCC->CR |= RCC_CR_HSEON;
	RCC->CR |= RCC_CR_HSION;
	while (!(RCC->CR & RCC_CR_HSERDY));

	RCC->CFGR |= RCC_CFGR_PLLSRC; // Configures the PLL to use HSE
	RCC->CFGR |= RCC_CFGR_PLLMULL9; // Multiplies the PLL by 9

	RCC->CR |= RCC_CR_PLLON; // Enable PLL after configuration
	while (!(RCC-> CR & RCC_CR_PLLRDY)); // Wait for PLL to be ready

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

	RCC->CFGR |= RCC_CFGR_SW_PLL;
	RCC->CFGR |= RCC_CFGR_MCOSEL_PLL_DIV2;

   /* Wait till System clock is ready */
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}