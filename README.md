# Brake Fluid Flush Machine

Brake Fluid Flush Machine is a personal project that was born in the need to replace an old and unfunctional PCB that controlled all the functions in an ATE FB30S.

All the mechanical parts of the machine were working perfectly, but the PBC broke down, not being able to control the output pressure anymore, going to full pressure constantly. That resulted in the necessity of a person (me, ha) continually controlling the pressure manually, simply and boringly turning the knob on and off, while flushing the brake fluid of the cars at the shop were I work.

The original PCB had to be completely replaced for a hand-made one that houses a STM32F103C6T6 (A.K.A. BluePill) as the brain of the machine. Also minor additions and modifications had to be done to the original wiring and components.

## How it works

Schematics for the wiring can be found in the schematics folder of the project (both .kicad files and screen captures for quick reference).

The C code is pretty simple and straightforward, and also heavily commented, but basically it runs a program that:

1. Includes all the start-up functions.
2. Declares 3 constants that correspond to one, two and three bar of pressure after the ADC (Analog to digital converter). Values were checked via a manometer while reading the values debugging the STM32F1.
3. Sets the interval for an alarm/buzzer that is triggered when something is wrong (brake fluid tank level too low, or pressure out of normal values).
4. Initializes variables for brake fluid tank level and pressure.
5. Declares the functions to check if everything is ready to run, check level, and detect the required pressure from the knob.
6. Calls all the start-up functions (clock configuration, PWM Configuration, ADC configuration, enable and start.
7. Initilizes the variables pwm (%/duty for the pump), setPoint (or desired pressure), error (setPoint - pressure) and timerBuzzer to 0.
8. Starts the infinite loop which:
  - Checks if everything is ok to run (pressure reading and tank level).
    - If so, it turns on the safety relay that feeds the pump, checks the required pressure, calculates the adequate PWM, and sets that PWM to the output pin that controls the pump.
    - If something is wrong, it sets the PMW to 0, turns the relay off, turns the red LED on, the green LED off, and starts a counter to turn the buzzer on and off.
