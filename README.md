# Brake Fluid Flush Machine

Brake Fluid Flush Machine is a personal project that was born in the need to replace an old and unfunctional PCB that controlled all the functions in an ATE FB30S.

All the mechanical parts of the machine were working perfectly, but the PBC broke down, not being able to control the output pressure anymore, going to full pressure constantly. That resulted in the necessity of a person (me, ha) continually controlling the pressure manually, simply and boringly turning the knob on and off, while flushing the brake fluid of the cars at the shop were I work.

Due to the age of the machine, spare parts are not longer available, making it impossible to buy a replacement PCB.

The original PCB had to be completely replaced for a hand-made circuit board that houses a STM32F103C6T6 (A.K.A. BluePill) as the brain of the machine. Also minor additions and modifications had to be done to the original wiring and components.

## How it works

Schematics for the wiring can be found in the schematics folder of the project (both .kicad files and screen captures for quick reference).

The C code is pretty simple and straightforward, and also heavily commented, but basically it runs a program which:

1. Includes all the start-up functions.
2. Declares 3 constants that correspond to three different settings of pressure (0.4, 1.5 and 2.2 bar) after the ADC (Analog to digital converter).
3. Sets the interval for an alarm/buzzer that is triggered when something is wrong (brake fluid tank level too low, or pressure out of normal values).
4. Initializes variables for brake fluid tank level and pressure.
5. Declares the functions to check if everything is ready to run, check level, and detect the required pressure from the knob.
6. Calls all the start-up functions (clock configuration, PWM Configuration, ADC configuration, enable and start.
7. Initilizes the variables pwm (%/duty for the pump), setPoint (or desired pressure), error (setPoint - pressure) and timerBuzzer to 0.
8. Starts the infinite loop which:
  - Checks if everything is ok to run (pressure reading and tank level).
    - If so, it turns on the safety relay that feeds the pump, checks the required pressure, calculates the adequate PWM, and sets that PWM to the output pin that controls the pump.
    - If something goes wrong, it sets the PMW to 0, turns the relay off, turns the red LED on, the green LED off, and starts a counter to turn the buzzer on and off.

## Now some images
<div align="center">
 
 ## The problem with the original circuit board.
 The old PCB was dangerously ramping the pressure up constantly beyond safe limits.
 ![Too much pressure](https://lh3.googleusercontent.com/pw/AL9nZEXRBaJH9ohCdPlgKFwH1t0pso10AT9IdDXd32XFWP8sOzDWjYoQWosrIPxzIdbOwi0pZcDIjSmIqmVuvfYqQpDZy2yZhW7LI1qP5CoOKJOSNjdWEDuy77M2fp6gxVtXMeybodgNf6VzVpw-entdc4Ka=w500-h268-no)
 
 ## Prototyping at home.
 ![Prototyping at home](https://lh3.googleusercontent.com/pw/AL9nZEWRU2Fcxen2eu2Is5HhuXg29laV45PEWurHrB4cMVqvShevNhHxuINT2j2j0AUPqX7nn-F8M6niiDpOdiDkRKK_MPYfdzpoIrRbSukCzI6MCQOHsuT2Vh7tZbvD74anECra5FB5cbe-mvgIJMDzrYGT=w500-h375-no)
  
 ## The old PCB ready to come out.
![Old PCB](https://lh3.googleusercontent.com/pw/AL9nZEUgVGReLhrqR16zL7KPi0kJ3tDzJyL50DJT6V5VamN13YBF2kxTiJc6WLM347VGx549UuFdAJXR3m9obiW5IqaeX3L8vUuandymOf_PcKWV5lvItqAHTTmVo4XvODy86YN_z4QFmO7OTCPefcIpwHiC=w500-h375-no)

## The new circuit board ready to go in (not the final version sadly)
![New PCB first version](https://lh3.googleusercontent.com/pw/AL9nZEWlQwKa9mvBPK0lHeQAPv2v5QDge7iK-2Ut6saoNSOGZtoHICZLfWVQWFeHclv7XR0sEK-Re7MlcK8unCWdmWjMd75tyG5dVW03ts-QY-a7xWYATtBRRhGbsL0BMaeAS95lQR6QyN6tGiClqYRmzMpV=w500-h666-no)
 
After a couple of runs, the main transistor overheated and stopped working properly: a junction went short-circuit, sending the pump to full speed. The safety relay came into action turning the power supply off avoiding a probable explosion of the plastic reservoir for the brake fluid of the car I was working on at the moment, due to over-pressure.
  
Time to see what happened.
 
## Wrong power transistor handling = Long fall time = Overheating
  
On the first version of the circuit board I was using a pre-made MOSFET driver module that was near to its current limits, plus a way too high frequency choice to avoid noise from DC motor.
  
 ![Long transistor fall time](https://lh3.googleusercontent.com/pw/AL9nZEWR__fvAlwdjqLEswZq5bXalj9jBWitizKjrRxcW22vLfFT4RCG0umuXvR_K1CQqBUsz9WP0mg7YNO6Yz5sB7wCG8CrjDmdMP0ge4nJu21_317OVBSb8XMmoArsUtcqniB1m4W0kJRsxPGbVNFHimDm=w500-h375-no)
  
  
 ## Lower frequency + using an optocoupler with the right resistor = Perfect.
  
 Now with some changes done to the transistor driving circuit and modifiying the code to run the PWM at 1kHz instead of 10kHz made a nice waveform.
  
![Better transistor fall time](https://lh3.googleusercontent.com/pw/AL9nZEWjZTGV9HoeCx6PQg5539utHlpaje7viwZBzFvThsI6Zbbn0LUWKaMz-h2chkAuermlV-horm4cg4er4QKs8hFCNDOKPOrZM0AgBUqUrJeYAavauK_L8QcAqbFQze-wpjQuHof22aRtzns6sjLA7CKK=w500-h375-no)
  
 ## Fitting the new version of the circuit board.
  
  ![Fitting the circuit baord](https://lh3.googleusercontent.com/pw/AL9nZEVhXML6teN9D8WFxLRrCtGcHiNwkafbbS5bBndphxcJm85gwBH5UfikhcckX74Rlyg32bSiSsGTxRiFQIc7pF8QG8xxla6CsTXdVT4zpvkJfYMzVe-KxI0ktIr9PvS41mBx2Hqt_3dPNJlSCxyH1pYt=w500-h375-no)
  
 ## Testing the 3 different pressure settings.
  
  ![Testing pressure](https://lh3.googleusercontent.com/pw/AL9nZEX7leTONf_0Gxw2vTLoS7Tc9oe3nc9U1Z_Wb7080bV5z0qk21aXCUL-fIcedpQSiJT1eWXwb2gHSIy54FFbqZqfn5aFsLDgXnGi7ZR1MBTgiN5aPfaBE9jM65Jl4RRJeb3-kgKMR7DE2kWzosP-gUuy=w500-h236-no)
  
## Brake fluid level in the tank going low.
  
(Note the pressure manometer dropping to 0 and the red LED turning on while the green one turns off, as well as the brake fluid stops coming out from the fitting of course).
  
  ![Low level testing](https://lh3.googleusercontent.com/pw/AL9nZEWE6f357kbiizwjcGf4m-DmnHLQlDFyPAC6Wt0Tfi_8C7hAiaVk3D8vUk3hiOS7uAcg8PgoIdbuOk2ywaGXgbogX9hXhbdQD9HVb20neNaj_DJCIL3v-LMwbO5QbwCtZxbJKTtqvY3hJNgZUxDxbEV-=w500-h587-no)
  
  
</div>
