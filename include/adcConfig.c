#include "stm32f103x6.h"

// Max ADC Clock frequency is 14 MHz
// ADCCLK is synchronous with PCLK2(APB2 Clock), which is set to 72MHz
// So I should use a prescaler of /6 to have the ADCCLK running at 12MHz
// Select Channel
// Conversion mode

void adcConfig(void)
{

        /*
        STEPS TO FOLLOW:
        1. Enable ADC and GPIO Clock
        2. Set the prescaler in the Common Control Register (CCR)
        3. Set the Scan Mode and resolution in the Control Register 1 (CR1)
        4. Set the Continuous Conversion, EOC, and Data Alignment in Control Red 2 (CR2)
        5. Set the Sampling Time for the cannels in ADC_SMPRx
        6. Set the Regular Channel sequence length in ADC_SQR1
        7. Set the Respective GPIO PINs in the Analog Mode
        */

    //1. Enable ADC and GPIO Clock
    // I'll enable ADC1EN
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;


    // Port A is already ON in gpioConfig.c
    //RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;


    //2. Set the prescaler in the Common Control Register (CCR)
    // ADC PRE is in RCC_CFGR
    RCC->CFGR &= ~(RCC_CFGR_ADCPRE_0);
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;

    // Now move to ADC registers
    //3. Set the Scan Mode and resolution in the Control Register 1 (CR1)
    ADC1->CR1 |= ADC_CR1_SCAN;
    //In the STM32F1 there is no configuration for resolution

    //4. Set the Continuous Conversion, EOC, and Data Alignment in Control Red 2 (CR2)

    // Continuous Conversion
    ADC1->CR2 |= ADC_CR2_CONT;
    // Data Alignment
    ADC1->CR2 &= ~ (ADC_CR2_ALIGN);

    // EOC? Seems to be default to 1 on STM32F1.
    // EOC Says: End of conversion
    //This bit is set by hardware at the end of a group channel conversion 
    // (regular or injected). It is cleared by software or by reading the ADC_DR.
    
    // Set External Event Selection pointed to SWSTART bit
    ADC1->CR2 |= ADC_CR2_EXTSEL;

    //5. Set the Sampling Time for the cannels in ADC_SMPRx
    // I'll be using ADCSMP1. Clear all the bits to get 1.5 cycles of Sample Time.
    ADC1->SMPR2 &= ~(ADC_SMPR2_SMP1);

    //6. Set the Regular Channel sequence length in ADC_SQR1
    // L[3:0]: Regular channel sequence length (How many ADC Channels I'm using)
    // 0000: 1 Conversion, so lets clear all the bits to 0.
    ADC1->SQR1 &= ~((ADC_SQR1_L));

    //7. Set the Respective GPIO PINs in the Analog Mode
    GPIOA->CRL &= ~(((GPIO_CRL_CNF1)) | (GPIO_CRL_MODE1));
    ADC1->SQR3 |= (1<<0);
}

void adcEnable(void)
{
    /*
    STEPS TO FOLLOW
    1. Enable the ADC by setting ADON bit in CR2
    2. Wait for ADC to stabilize (approx 10us)
    */

   ADC1->CR2 |= ADC_CR2_ADON; // ADON = 1 Enable ADC1

    uint32_t delay = 10000;
    while (delay--);

    ADC1->CR2 |= ADC_CR2_CAL;

    uint32_t delay2 = 10000;
    while (delay2--);
}

void adcStart(int channel)
{
    /* 
    STEPS TO FOLLOW
    1. Set the channel sequence in the SQR Register
    2. Clear the Status Register
    3. Start the Conversion by settin the SWSTART bit in CR2
    */
   	ADC1->SR = 0;                      // Clear Status register
	ADC1->CR2 |= (1<<20);              // Conversion on external event enabled
	ADC1->CR2 |= 1<<22;                // Start conversion

}

void adcWaitForConv(void)
{
    // EOC Flag will be set, once the conversion is finished
    while (!(ADC1->SR & (1<<1)));
}

uint16_t adcGetVal(void)
{
    // Read the data register
    return ADC1->DR;
}

void adcDisable(void)
{
    ADC1->CR2 &= ~(1<<0); // Disable ADC
}