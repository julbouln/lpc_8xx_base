#include "Arduino.h"
#include "HardwareSerial.h"

int main(void)
{
		SystemCoreClockUpdate();
		SysTick_Config(SystemCoreClock / 1000);

		Chip_GPIO_Init(LPC_GPIO_PORT);
				
		/* Enable the clock to the Switch Matrix */
		Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);		
				
        setup();
    
		/* Disable the clock to the Switch Matrix to save power */
		Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
	
        while (1) {
            loop();
        }
        
        return 0;
}