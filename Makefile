OPTIMIZATION = s

# lpc810 / lpc811 / lpc812
MCU=lpc812

#########################################################################
PROJECT=template
LDSCRIPT=core/$(MCU).ld


C_SRC=$(wildcard core/*.c) $(wildcard chip_8xx/src/*.c) $(wildcard arduino/*.c)
CXX_SRC=$(wildcard core/*.cpp) $(wildcard *.cpp) $(wildcard arduino/*.cpp) $(wildcard libraries/*/*.cpp)

LIBRARIES=$(wildcard libraries/*)

C_OBJECTS=$(C_SRC:.c=.o)
C_DEPS=$(C_SRC:.c=.d)
C_LSTFILES=$(C_SRC:.c=.lst)

CXX_OBJECTS=$(CXX_SRC:.cpp=.o)
CXX_DEPS=$(CXX_SRC:.cpp=.d)
CXX_LSTFILES=$(CXX_SRC:.cpp=.lst)

#  Compiler Options
GCFLAGS = -Os --specs=nano.specs 
#-flto 
GCFLAGS += -ffunction-sections -fdata-sections -fno-builtin
GCFLAGS += -mthumb -mcpu=cortex-m0plus
GCFLAGS += -fno-rtti -fno-exceptions -fpermissive 
# who needs precision maths with this sort of chip ?
GCFLAGS += -ffast-math -fshort-double
GCFLAGS += -DFLOAT_ONLY -D__USE_CMSIS -DUSE_ROM_API -DARDUINO=100 -DMCU$(MCU)
GCFLAGS += -Ichip_8xx/inc -Iarduino $(foreach d, $(LIBRARIES), -I$d)

LDFLAGS = -T$(LDSCRIPT) -Wl,-M,--gc-section > firmware.map

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
	@$(OBJCOPY) --strip-unneeded -R .stack -O binary $(PROJECT).elf firmware.bin


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
#	lpc21isp firmware.hex /dev/cu.usbserial-AM01R02J 115200 14746
	tools/mcp2200_isp
	lpc21isp firmware.hex /dev/cu.usbmodem1411 115200 14746
	tools/mcp2200_boot
.PHONY : clean all flash stats debug

