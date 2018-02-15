################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include example/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: E4ISD2_can_Team1.axf

# Tool invocations
E4ISD2_can_Team1.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/tobias/Documents/MCUXpresso_10.1.0_589/workspace/lpc_chip_40xx/Debug" -L"/home/tobias/Documents/MCUXpresso_10.1.0_589/workspace/lpc_board_ea_devkit_4088/Debug" -Xlinker -Map="E4ISD2_can_Team1.map" -Xlinker --gc-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -T "E4ISD2_can_rx_tx_Debug.ld" -o "E4ISD2_can_Team1.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) E4ISD2_can_Team1.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "E4ISD2_can_Team1.axf"; # arm-none-eabi-objcopy -v -O binary "E4ISD2_can_Team1.axf" "E4ISD2_can_Team1.bin" ; # checksum -p LPC4088 -d "E4ISD2_can_Team1.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
