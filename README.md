[NXP's LPC800 family microcontroller](http://www.nxp.com/products/microcontrollers/cortex_m0_m0/lpc800/) development code base.

The code should work with LPC810, LPC811 and LPC812.

The code is based on [LPCOpen 2](http://www.lpcware.com/content/nxpfile/lpcopen-software-development-platform-lpc8xx-packages) source code and [sebseb7/lpc8xx Makefile](https://github.com/sebseb7/lpc8xx) (arm-none-eabi toolchain). The main addition is C++ capability and an Arduino like compatible API.

Supported Arduino functions and libraries :

* pinMode, digitalWrite, digitalRead
* delay
* Serial
* SPI
* Wire

INO files can be compiled with ino2firmware.sh script.

Code come with an hardware implementation composed of a MCP2200 USB to UART and a LPC812 chip.
https://github.com/julbouln/mcp2200_simpleio and lpc21isp must be installed and tools in tools/ compiled to use "make flash" which upload firmware.hex.