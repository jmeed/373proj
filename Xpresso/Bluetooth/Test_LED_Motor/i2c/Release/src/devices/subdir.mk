################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/devices/accelerometer.c \
../src/devices/bluetooth.c \
../src/devices/fuel_gauge.c \
../src/devices/i2c_com.c \
../src/devices/joystick.c \
../src/devices/screen.c \
../src/devices/vibrator.c 

OBJS += \
./src/devices/accelerometer.o \
./src/devices/bluetooth.o \
./src/devices/fuel_gauge.o \
./src/devices/i2c_com.o \
./src/devices/joystick.o \
./src/devices/screen.o \
./src/devices/vibrator.o 

C_DEPS += \
./src/devices/accelerometer.d \
./src/devices/bluetooth.d \
./src/devices/fuel_gauge.d \
./src/devices/i2c_com.d \
./src/devices/joystick.d \
./src/devices/screen.d \
./src/devices/vibrator.d 


# Each subdirectory must supply rules for building sources it contributes
src/devices/%.o: ../src/devices/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS -DNDEBUG -D__CODE_RED -I../cmsis -I../config -I../driver -O2 -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


