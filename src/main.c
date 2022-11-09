#include "includes.h"

// The next values are ADC values that correspond to 1, 2 and 3 bars of pressure.
const uint16_t oneBar = 800;
const uint16_t twoBar = 1100;
const uint16_t threeBar = 1400;

// This value stores how often the buzzer will toggle (in 10^(-4)/0.1ms increments).
const uint16_t toggleInterval = 10000;

volatile uint8_t levelOk = 0;       // Brake fluid tank level
volatile uint16_t pressure = 0;     // Process Variable/Actual Pressure

// Function that will check if everything is in order to work
uint8_t allGood(void);
// Function that will check the tank level accessing the Port A Pin 8 Input Data Register
uint8_t checkLevel(void);
// Function that will update the set point based on the position of the knob
uint16_t requiredPressure(void);

int main(void)
{
    // Initial configuration functions
  	systemClockConfig();
	pwmConfig();
	gpioConfig();
    adcConfig();
    adcEnable();
    adcStart(1);
	
	volatile uint16_t pwm = 0;          // Control Variable/PWM
    volatile uint16_t setPoint = 0;     // Set Point/Desired pressure
    volatile int16_t error = 0;         // Set Point - Process Variable
    volatile uint16_t timerBuzzer = 0;  // Counter to toggle the buzzer 

    while (1)
	{

        //levelOk = checkLevel();     // Check if the tank level is ok and set as variable

        //pressure = adcGetVal();                     // Get the actual pressure from the ADC

        // If brake fluid tank level is ok, and the pressure sensor is between expected/normal values...
        if (allGood())
        {
            TIM2->CCR3 = 0;                         // Turn the buzzer OFF

            GPIOA->ODR &= ~GPIO_ODR_ODR7;           // Turn red led OFF

            GPIOA->ODR |= GPIO_ODR_ODR6;            // Turn green led ON

            GPIOA->ODR |= GPIO_ODR_ODR9;            // Turn on the power relay that feeds the motor driver

            // While everything is in order...
            while (allGood())
            {
                setPoint = requiredPressure();          // Update the set point to the selected pressure on the machine front knob

                error = setPoint - pressure;            // Calculate the error

                pwm = error * 0.2;                      // Calculate PWM duty - 0.2 is kp/Proportional gain

                if ((pwm > 20) & (pwm < 100))
                {
                    TIM2->CCR1 = pwm;                   // Set the value calculated on pwm as the timer duty cycle
                }
                else if (pwm > 100)
                {
                    TIM2->CCR1 = 100;                   // If calculated value is bigger than 100, set it at 100% (Maximum duty)
                }
                else
                {
                    TIM2->CCR1 = 0;                     // If calculated value is smaller than 0, leave it at 0% (Minimum duty)
                }
            }
        }
        // If conditions were not met...
        else
        {
            GPIOA->ODR &= ~GPIO_ODR_ODR9;            // Turn off the power relay that feeds the motor driver

            GPIOA->ODR &= ~GPIO_ODR_ODR6;           // Turn green led ON

            GPIOA->ODR |= GPIO_ODR_ODR7;            // Turn red led OFF

            pwm = 0;                                // Set PWM to 0%

            TIM2->CCR1 = pwm;                       // Set time Duty Cycle to 0%

            timerBuzzer = toggleInterval - 500;     // Force the timer to be close to 9999 to start as soon as possible.

            /* Here I'm using Timer 2 Update Interrupt Flag as a timer. 
            I don't expect this method to be 100% accurate but it's enough to toggle...
            ... a buzzer on and off, non-blocking, and without activating another Timer.  
            TIM2 UIF is set to 1 by hardware every time TIM2 CNT reaches TIM2 ARR (100)
            Being TIM2 frequency = 1Mhz, CNT will be increased each uS, and TIM2 UIF...
            ... will be set to 1 every 100uS
            That means that every 10000 UIF events, 1 million uS/1 second has passed. */
            
            // And while tank level is low, or the pressure is out of range, simply toggle the buzzer 
            while (!(allGood()))
            {
                if (TIM2->SR & TIM_SR_UIF)              
                {
                    TIM2->SR &= ~TIM_SR_UIF;
                    
                    timerBuzzer++;
                }

                // If 1 second has passed, toggle the buzzer and reset counter/timer to 0.
                if (timerBuzzer > toggleInterval)
                {
                    if (TIM2->CCR3 != 0)
                    {
                        TIM2->CCR3 = 0;
                    }
                    else
                    {
                        TIM2->CCR3 = 50;
                    }
                timerBuzzer = 0;
                }
            }
        }
	}
}

uint8_t allGood(void)
{
    pressure = adcGetVal();     // Get the actual pressure from the ADC

    levelOk = checkLevel();     // Check if the tank level is ok

    if ((levelOk) && (pressure >= 300) && (pressure <= 2000)) 
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

uint8_t checkLevel(void)
{
    if ((GPIOA->IDR & GPIO_IDR_IDR8) != 0)  // Read the status of the level sensor from Port A Input Data Register 8
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint16_t requiredPressure(void)
{
    if (GPIOA->IDR & GPIO_IDR_IDR3)                 // If Pin 3 is ON
    {
        return oneBar;
    }
    else if (GPIOA->IDR & GPIO_IDR_IDR4)            // If Pin 4 is ON
    {
        return twoBar;
    }
    else if (GPIOA->IDR & GPIO_IDR_IDR5)            // If Pin 5 is ON
    {
        return threeBar;
    }
    //Otherwise, the machine is not required to work, so set the Set Point to 0
    else
    {
        return 0;
    }
}

