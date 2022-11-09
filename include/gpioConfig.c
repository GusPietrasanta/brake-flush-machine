#include "stm32f103x6.h"

void gpioConfig(void)
{
    // Enable GPIOA clock and configure output pin
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	// FIRST RESET THE BITS TO 0!!!
	GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    // FIRST RESET THE BITS TO 0!!!
    GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);

	// Configure GPIOA Pin0 (PWM Signal) Mode as Output Max Speed (50Mhz) AND GPIOA Pin2 (Buzzer) Mode as Output Max Speed (50Mhz)
	GPIOA->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0 | GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0);
	// Configure GPIOA Pin0 (PWM Signal) Output Mode as Alternate Function Push-Pull AND GPIOA Pin2 (Buzzer) Output Mode as Alternate Function Push-Pull 
  	GPIOA->CRL |= ((GPIO_CRL_CNF0_1) | (GPIO_CRL_CNF2_1));

    // Configure GPIOA Pin3 (1Bar Request = ADC to 800) as Input and Pull-down
    GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
    GPIOA->CRL |= (GPIO_CRL_CNF3_1);
    GPIOA->ODR &= ~(GPIO_ODR_ODR3);

    // Configure GPIOA Pin4 (2Bar Request = ADC to 1100) as Input and Pull-down
    GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
    GPIOA->CRL |= (GPIO_CRL_CNF4_1);
    GPIOA->ODR &= ~(GPIO_ODR_ODR4);

    // Configure GPIOA Pin5 (3Bar Request = ADC to 1400) as Input and Pull-down
    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
    GPIOA->CRL |= (GPIO_CRL_CNF5_1);
    GPIOA->ODR &= ~(GPIO_ODR_ODR5);

    // Configure GPIOA Pin6 (Green Led) as Output
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
    GPIOA->CRL |= (GPIO_CRL_MODE6_1);
    GPIOA->ODR &= ~(GPIO_ODR_ODR6);

    // Configure GPIOA Pin7 (Red Led) as Output
    GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
    GPIOA->CRL |= (GPIO_CRL_MODE7_1);
    GPIOA->ODR &= ~(GPIO_ODR_ODR7);

    // Configure GPIOA Pin8 (Level Sensor) as Input and Pull-down
    GPIOA->CRH &= ~(GPIO_CRH_MODE8 | GPIO_CRH_CNF8);
    GPIOA->CRH |= (GPIO_CRH_CNF8_1);
    GPIOA->ODR &= ~(GPIO_ODR_ODR8);

    // Configure GPIOA Pin9 (Relay) as Output
    GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);
    GPIOA->CRH |= (GPIO_CRH_MODE9_1);
    GPIOA->ODR &= ~(GPIO_ODR_ODR9);
}