################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/can.c \
../driver/crp.c \
../driver/gpio.c \
../driver/i2c.c \
../driver/uart.c 

OBJS += \
./driver/can.o \
./driver/crp.o \
./driver/gpio.o \
./driver/i2c.o \
./driver/uart.o 

C_DEPS += \
./driver/can.d \
./driver/crp.d \
./driver/gpio.d \
./driver/i2c.d \
./driver/uart.d 


# Each subdirectory must supply rules for building sources it contributes
driver/%.o: ../driver/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS -DNDEBUG -D__CODE_RED -I../cmsis -I../config -I../driver -O2 -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


