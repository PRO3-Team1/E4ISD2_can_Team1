################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/button.c \
../example/src/can.c \
../example/src/cr_startup_lpc40xx.c \
../example/src/sysinit.c 

OBJS += \
./example/src/button.o \
./example/src/can.o \
./example/src/cr_startup_lpc40xx.o \
./example/src/sysinit.o 

C_DEPS += \
./example/src/button.d \
./example/src/can.d \
./example/src/cr_startup_lpc40xx.d \
./example/src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M4 -I"/home/tobias/Documents/MCUXpresso_10.1.0_589/workspace/lpc_chip_40xx/inc" -I"/home/tobias/Documents/MCUXpresso_10.1.0_589/workspace/lpc_board_ea_devkit_4088/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


