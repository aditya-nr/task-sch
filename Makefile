SRC=src/01*.c
TARGET=firmware.elf
BUILD_DIR=.
CC=arm-none-eabi-gcc
DIS=arm-none-eabi-objdump
DIS_TARGET=firmware_disassembly.txt

CC_FLAGS=-Wall -mcpu=cortex-m3 -mthumb -nostartfiles
DEBUG_FLAGS=-O0 -g3 -gdwarf-2
LINKER_FLAGS=-specs=rdimon.specs -lc -lnosys

STARTUP_SCRIPT=lib/startup_stm32f103c6tx.s
LINKER_SCRIPT=lib/STM32F103C6TX_FLASH.ld
INIT_SCRIPT=lib/init.c


all: $(BUILD_DIR)/$(TARGET) 

build: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(SRC)
	$(CC) $(CC_FLAGS) $(DEBUG_FLAGS) \
	-T $(LINKER_SCRIPT) $(LINKER_FLAGS) \
	-o $(BUILD_DIR)/$(TARGET) \
	$(SRC) $(INIT_SCRIPT) $(STARTUP_SCRIPT) 

flash:
	STM32_Programmer_CLI -c port=SWD -w $(BUILD_DIR)/$(TARGET) 0x08000000 -v

gdb:
	arm-none-eabi-gdb $(BUILD_DIR)/$(TARGET)

openocd:
	openocd \
	-f /usr/share/openocd/scripts/interface/stlink.cfg \
	-f /usr/share/openocd/scripts/target/stm32f1x.cfg 

disassemble: $(SRC)
	$(CC) -c -g3 -gdwarf-2 $(SRC) -o temp/$(DIS_TARGET).o
	$(DIS)  -S --source-comment="code : " temp/$(DIS_TARGET).o  > $(DIS_TARGET)
clean:
	rm *.elf

temp:
	arm-none-eabi-gcc -c -std=gnu11 -O0 -g3 -mcpu=cortex-m3 -mthumb \
	src/13_stacking.c -o main.elf
temp2:
	arm-none-eabi-objdump -S --source-comment=code main.elf > main.txt