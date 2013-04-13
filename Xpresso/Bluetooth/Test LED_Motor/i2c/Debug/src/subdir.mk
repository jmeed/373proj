################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/globals.c \
../src/headlines.c \
../src/i2c_main.c \
../src/screen.c \
../src/shared.c \
../src/snake.c \
../src/watch.c \
../src/weather.c 

OBJS += \
./src/globals.o \
./src/headlines.o \
./src/i2c_main.o \
./src/screen.o \
./src/shared.o \
./src/snake.o \
./src/watch.o \
./src/weather.o 

C_DEPS += \
./src/globals.d \
./src/headlines.d \
./src/i2c_main.d \
./src/screen.d \
./src/shared.d \
./src/snake.d \
./src/watch.d \
./src/weather.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS -DDEBUG -D__CODE_RED -I../cmsis -I../config -I../driver -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Werror -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


