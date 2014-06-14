#include "SPI.h"
#include "HardwareSerial.h"

SPI_CONFIG_T ConfigStruct;
static SPI_DELAY_CONFIG_T DelayConfigStruct;

// Constructors ////////////////////////////////////////////////////////////////

SPIClass::SPIClass(int spi_n)
{
	_spi_n=spi_n;
}

void SPIClass::begin() {
	_spi=(_spi_n) ? (LPC_SPI_T *)(LPC_SPI1_BASE) : (LPC_SPI_T *)(LPC_SPI0_BASE);
	
		ConfigStruct.Mode = SPI_MODE_MASTER;
		// default 100khz
		ConfigStruct.ClkDiv = Chip_SPI_CalClkRateDivider(_spi, 100000);
		ConfigStruct.ClockMode = SPI_CLOCK_CPHA0_CPOL0;
		ConfigStruct.DataOrder = SPI_DATA_MSB_FIRST;
		ConfigStruct.SSELPol = SPI_SSEL_ACTIVE_LO;
		Chip_SPI_Init(_spi, &ConfigStruct);

		DelayConfigStruct.FrameDelay = 0;
		DelayConfigStruct.PostDelay = 0;
		DelayConfigStruct.PreDelay = 0;
		DelayConfigStruct.TransferDelay = 0;
		Chip_SPI_DelayConfig(_spi, &DelayConfigStruct);

		Chip_SPI_Enable(_spi);

}


void SPIClass::end() {
	Chip_SPI_DeInit(_spi);
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
	switch(bitOrder) {
		case LSBFIRST:
		_spi->CFG |= SPI_DATA_LSB_FIRST;
		case MSBFIRST:
		_spi->CFG |= SPI_DATA_MSB_FIRST;
		
	}
	
}

void SPIClass::setDataMode(uint8_t mode)
{
	// TODO
}

void SPIClass::setClockDivider(uint8_t rate)
{
	_spi->DIV = SPI_DIV_VAL(Chip_SPI_CalClkRateDivider(_spi, rate));
}


byte SPIClass::transfer(byte _data) {
	uint32_t data;

	_spi->TXCTRL = SPI_TXDATCTL_FLEN(7);
	
	while(~_spi->STAT & SPI_STAT_TXRDY);
	_spi->TXDATCTL = SPI_TXDATCTL_EOT | SPI_TXDATCTL_FLEN(7) |  _data; 

	while(~_spi->STAT & SPI_STAT_RXRDY);
	data = _spi->RXDAT;
		
	return data & 0xFF;
}

void SPIClass::attachInterrupt() {
}

void SPIClass::detachInterrupt() {
}


SPIClass SPI(0);
SPIClass SPI1(1);
