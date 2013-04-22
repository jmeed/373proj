################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/screens/debug.c \
../src/screens/headlines.c \
../src/screens/music.c \
../src/screens/snake.c \
../src/screens/watch.c \
../src/screens/weather.c 

OBJS += \
./src/screens/debug.o \
./src/screens/headlines.o \
./src/screens/music.o \
./src/screens/snake.o \
./src/screens/watch.o \
./src/screens/weather.o 

C_DEPS += \
./src/screens/debug.d \
./src/screens/headlines.d \
./src/screens/music.d \
./src/screens/snake.d \
./src/screens/watch.d \
./src/screens/weather.d 


# Each subdirectory must supply rules for building sources it contributes
src/screens/%.o: ../src/screens/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS -DNDEBUG -D__CODE_RED -I../cmsis -I../config -I../driver -O2 -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


