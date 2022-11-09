#include "stm32f103x6.h"

void pwmConfig(void)
{
    // First, make sure the timer is off
	TIM2->CR1 	&= ~(TIM_CR1_CEN);
	TIM2->SR 	&= ~(TIM_SR_UIF);

	// Reset TIM2 BUS
	RCC->APB1RSTR 	|= (RCC_APB1RSTR_TIM2RST);
	RCC->APB1RSTR 	&= ~(RCC_APB1RSTR_TIM2RST);


	// Enable TIM2 Clock
  	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB2ENR_AFIOEN;

	// Enable preload for CCR1 register
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM2->CCMR2 |= TIM_CCMR2_OC3PE;

	// Enable preload for ARR register
    TIM2->CR1 |= TIM_CR1_ARPE;

	// Set TIM2C1 as output for PWM mode
    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;
	// Set TIM2C3 as output for PWM mode
    TIM2->CCMR2 &= ~TIM_CCMR2_CC3S;

	// Enable TIM2C1
	TIM2->CCER |= TIM_CCER_CC1E;
    // Enable TIM2C3
    TIM2->CCER |= TIM_CCER_CC3E;

	// Enable this bit so there will not be any register value updated
	TIM2->CR1 |= TIM_CR1_UDIS;

	// Timer prescaler (that's 72) will divide the BUS clock...
	// ... (72Mhz) and make it a 1Mhz clock
	TIM2->PSC = 71;
	// TIM2ARR will divide 1Mhz (TIM2 Clock) by 100...
	// ... and make the timer frequency 10khz
    TIM2->ARR = 100;
	// TIM2CCR1 is the "limit" were the output changes from high to low.
	// 0 means 0% duty, 50 is 50% duty and so on... 
    TIM2->CCR1 = 0;
    TIM2->CCR3 = 0;

	// Disable this bit so values assigned from timer register...
	// ... can move to preload registers
	TIM2->CR1 &= ~TIM_CR1_UDIS;

	// Enable the update generation updating the shadow register 
    TIM2->EGR |= TIM_EGR_UG;

	// Clear UIF flag to update content of the register
	TIM2->SR &= ~(TIM_SR_UIF);

    // Clear bits for channels mode configuration first
    TIM2->CCMR1 &= ~(TIM_CCMR1_OC1M);
    TIM2->CCMR1 &= ~(TIM_CCMR2_OC3M);
	// Set TIM2C1 as PWM Mode 1
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    // Set TIM2C3 as PWM Mode 1
    TIM2->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);

	// Set output signal polarity of channel 1 as active high
	TIM2->CCER &= ~(TIM_CCER_CC1P);
	// Set output signal polarity of channel 3 as active high
    TIM2->CCER &= ~(TIM_CCER_CC3P);

	// Enable the timer itself
    TIM2->CR1 |= TIM_CR1_CEN;
}