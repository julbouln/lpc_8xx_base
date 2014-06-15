OPTIMIZATION = s

# lpc810 / lpc811 / lpc812
MCU=lpc812

#########################################################################
PROJECT=template
LDSCRIPT=core/$(MCU).ld


C_SRC=$(wildcard core/*.c) $(wildcard chip_8xx/src/*.c) $(wildcard arduino/*.c)
CXX_SRC=$(wildcard *.cpp) $(wildcard arduino/*.cpp)

C_OBJECTS=$(C_SRC:.c=.o)
C_DEPS=$(C_SRC:.c=.d)
C_LSTFILES=$(C_SRC:.c=.lst)

CXX_OBJECTS=$(CXX_SRC:.cpp=.o)
CXX_DEPS=$(CXX_SRC:.cpp=.d)
CXX_LSTFILES=$(CXX_SRC:.cpp=.lst)

#  Compiler Options
GCFLAGS = -std=gnu99  -mcpu=cortex-m0plus -mthumb -O$(OPTIMIZATION) -I. -Ichip_8xx/inc -Iarduino
# Warnings
GCFLAGS += -Wno-strict-aliasing -Wstrict-prototypes -Wundef -Wall -Wextra -Wunreachable-code 
# Optimizazions
GCFLAGS +=  -fstrict-aliasing -fsingle-precision-constant -funsigned-char -funsigned-bitfields -fshort-enums -fno-builtin -ffunction-sections -fno-common -fdata-sections -fpermissive 
# Debug stuff
GCFLAGS += -Wa,-adhlns=$(<:.c=.lst) -g

GCFLAGS = -Os --specs=nano.specs -ffunction-sections -fdata-sections -fno-builtin -mthumb -mcpu=cortex-m0plus -MD   -Ichip_8xx/inc -Iarduino -D__USE_CMSIS -DMCU$(MCU) 

GCFLAGS += -fno-rtti -fno-exceptions

LDFLAGS = -T$(LDSCRIPT) -nostartfiles  -Wl,--gc-section 

#LDFLAGS = -T$(LDSCRIPT) -Wl,--gc-sections

#  Compiler/Linker Paths
GCC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
REMOVE = rm -f
SIZE = arm-none-eabi-size
GCXX = arm-none-eabi-g++

#########################################################################


all: checksource firmware.bin stats

checksource:
ifeq ($(wildcard *.cpp),)
	@echo "Missing toplevel C++ file"; 
	@echo "Use ./ino2firmware.sh file.ino"
	@exit 2;
endif
	
firmware.bin: $(PROJECT).elf Makefile
	@$(OBJCOPY) --strip-unneeded -O ihex $(PROJECT).elf firmware.hex
	@$(OBJCOPY) -R .stack -O binary $(PROJECT).elf firmware.bin


$(PROJECT).elf: $(C_OBJECTS) $(CXX_OBJECTS) Makefile
	@echo "  \033[1;34mLD \033[0m $(C_OBJECTS) $(CXX_OBJECTS) $(GCFLAGS) $(LDFLAGS) (\033[1;33m$(OBJECTS)\033[0m) -> $(PROJECT).elf"
	@$(GCXX) -o $(PROJECT).elf $(C_OBJECTS) $(CXX_OBJECTS) $(GCFLAGS) $(LDFLAGS) 

stats: $(PROJECT).elf 
	@$(SIZE) $(PROJECT).elf

clean:
	@echo "  \033[1;34mCleanup\033[0m $<"
	@$(REMOVE) $(C_OBJECTS)
	@$(REMOVE) $(C_DEPS)
	@$(REMOVE) $(C_LSTFILES)
	@$(REMOVE) $(CXX_OBJECTS)
	@$(REMOVE) $(CXX_DEPS)
	@$(REMOVE) $(CXX_LSTFILES)
	@$(REMOVE) firmware.bin
	@$(REMOVE) firmware.hex
	@$(REMOVE) template.elf
	@$(REMOVE) $(PROJECT).el
	@$(REMOVE) *.d
	@$(REMOVE) *.o

-include $(C_DEPS)

#########################################################################

%.o: %.c Makefile 
	@echo "  \033[1;34mGCC\033[0m $(GCC) $(GCFLAGS)$<"
	@$(GCC) $(GCFLAGS) -o $@ -c $<

%.o: %.cpp Makefile 
	@echo "  \033[1;34mGCC\033[0m $(GCXX) $(GCFLAGS)$<"
	@$(GCXX) $(GCFLAGS) -o $@ -c $<

#########################################################################

flash:
	lpc21isp firmware.hex /dev/cu.usbserial-AM01R02J 115200 14746

.PHONY : clean all flash stats debug

