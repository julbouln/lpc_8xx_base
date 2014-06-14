#include "chip_8xx.h"

/*
* datasheet: http://www.nxp.com/documents/data_sheet/LPC81XM.pdf
* user manual: http://www.nxp.com/documents/user_manual/UM10601.pdf
* swtich matrix tool: http://www.lpcware.com/content/nxpfile/nxp-switch-matrix-tool-lpc800 (for pin configuration)
*
*
* ISP: When pin PIO0_1 is pulled LOW on reset, the part enters ISP mode and the ISP command handler starts up.
* In ISP mode, pin PIO0_0 is connected to function U0_RXD and pin PIO0_4 is connected to function U0_TXD on the USART0 block.
*/



/* Tx buffer */
static uint16_t TxBuf[10];

/* Rx buffer */
static uint16_t RxBuf[10];

static SPI_CONFIG_T ConfigStruct;
static SPI_DELAY_CONFIG_T DelayConfigStruct;

static SPI_DATA_SETUP_T XfSetup;
static volatile uint8_t  isXferCompleted = 0;


static void bufferInit(void)
{
        uint16_t i;
        uint16_t ch = 0;

        for (i = 0; i < 10; i++) {
                TxBuf[i] = ch++;
                RxBuf[i] = 0xAA;
        }
}

/* Verify buffer after transfer */
static uint8_t bufferVerify(void)
{
        uint16_t i;
        uint16_t *src_addr = (uint16_t *) &TxBuf[0];
        uint16_t *dest_addr = (uint16_t *) &RxBuf[0];

        for ( i = 0; i < 10; i++ ) {

                if (*src_addr != *dest_addr) {
                        return 1;
                }
                src_addr++;
                dest_addr++;
        }
        return 0;
}

#define LED1 16

int main(void) {
SystemCoreClockUpdate();
SysTick_Config(SystemCoreClock / 1000);
Chip_GPIO_Init(LPC_GPIO_PORT);

/* Enable the clock to the Switch Matrix */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);
	/*
	 * Initialize SPI0 pins connect
	 */
	Chip_SWM_DisableFixedPin(SWM_FIXED_VDDCMP);
	Chip_SWM_MovablePinAssign(SWM_SPI0_SCK_IO, 1);
	Chip_SWM_MovablePinAssign(SWM_SPI0_MOSI_IO, 20);
	Chip_SWM_MovablePinAssign(SWM_SPI0_MISO_IO, 11);
	Chip_SWM_MovablePinAssign(SWM_SPI0_SSEL_IO, 2);
	
	ConfigStruct.Mode = SPI_MODE_MASTER;
	ConfigStruct.ClkDiv = Chip_SPI_CalClkRateDivider(LPC_SPI0, 100000);
	ConfigStruct.ClockMode = SPI_CLOCK_CPHA0_CPOL0;
	ConfigStruct.DataOrder = SPI_DATA_MSB_FIRST;
	ConfigStruct.SSELPol = SPI_SSEL_ACTIVE_LO;
	Chip_SPI_Init(LPC_SPI0,&ConfigStruct);

	DelayConfigStruct.FrameDelay = 0;
	DelayConfigStruct.PostDelay = 0;
	DelayConfigStruct.PreDelay = 0;
	DelayConfigStruct.TransferDelay = 0;
	Chip_SPI_DelayConfig(LPC_SPI0, &DelayConfigStruct);
	
	Chip_SPI_Enable(LPC_SPI0);

	Chip_SPI_EnableLoopBack(LPC_SPI0);
	
	bufferInit();
	
	XfSetup.Length = 10;
    XfSetup.pTx = TxBuf;
    XfSetup.RxCnt = XfSetup.TxCnt = 0;
	XfSetup.DataSize = 8;
	XfSetup.pRx = RxBuf;
	
	Chip_SPI_RWFrames_Blocking(LPC_SPI0, &XfSetup);
   
	if (bufferVerify()) {
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,0,LED1,true);
		}
	        else
		{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,0,LED1,false);
		}
   
   
	Chip_GPIO_SetPinDIR(LPC_GPIO_PORT,0,LED1,true);

	Chip_SPI_DeInit(LPC_SPI0);
	
	while (1)
	{
//		Chip_GPIO_SetPinState(LPC_GPIO_PORT,0,LED1,true);
//		delay_ms(1000);
//		Chip_GPIO_SetPinState(LPC_GPIO_PORT,0,LED1,false);
//		delay_ms(1000);
	}

}

