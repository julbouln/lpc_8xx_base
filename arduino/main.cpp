#include "Arduino.h"
#include "HardwareSerial.h"

int main(void)
{
		SystemCoreClockUpdate();
		SysTick_Config(SystemCoreClock / 1000);

 
		Chip_GPIO_Init(LPC_GPIO_PORT);
				
		Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);

		Chip_Clock_SetUARTClockDiv(1);	/* divided by 1 */

		/* Connect the U0_TXD_O and U0_RXD_I signals to port pins(P0.4, P0.0) */
		Chip_SWM_DisableFixedPin(SWM_FIXED_ACMP_I1);
		Chip_SWM_MovablePinAssign(SWM_U0_TXD_O, 4);
		Chip_SWM_MovablePinAssign(SWM_U0_RXD_I, 0);
	
		/* SPI0_SCK 1 P0.17 */
		/* SPI0_MOSI 20 P0.14 */
	    /* SPI0_MISO 11 P0.15 */
	    /* SPI0_SSEL 2 P0.13 */
		
		Chip_SWM_DisableFixedPin(SWM_FIXED_VDDCMP);
		Chip_SWM_MovablePinAssign(SWM_SPI0_SCK_IO, 17);
		Chip_SWM_MovablePinAssign(SWM_SPI0_MOSI_IO, 14);
		Chip_SWM_MovablePinAssign(SWM_SPI0_MISO_IO, 15);
//		Chip_SWM_MovablePinAssign(SWM_SPI0_SSEL_IO, 13);
		
		
        setup();
    
        for (;;) {
                loop();
        }
        
        return 0;
}