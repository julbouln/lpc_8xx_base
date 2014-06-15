#include "Arduino.h"
#include "HardwareSerial.h"

int main(void)
{
		SystemCoreClockUpdate();
		SysTick_Config(SystemCoreClock / 1000);

		Chip_GPIO_Init(LPC_GPIO_PORT);
				
		Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);		
				
        setup();
    
        for (;;) {
                loop();
        }
        
        return 0;
}