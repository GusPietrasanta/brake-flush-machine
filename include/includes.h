#include "gpioConfig.c"
#include "clockConfig.c"
#include "pwmConfig.c"
#include "adcConfig.c"

void clockConfig(void);
void gpioConfig(void);
void pwmConfig(void);
void adcConfig(void);
void adcEnable(void);
void adcStart(int channel);
void adcWaitForConv(void);
uint16_t adcGetVal(void);