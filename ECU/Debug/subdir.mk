################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ECU.c \
../ICU.c \
../TIMERS.c \
../TWI.c \
../UART.c \
../external_eeprom.c 

OBJS += \
./ECU.o \
./ICU.o \
./TIMERS.o \
./TWI.o \
./UART.o \
./external_eeprom.o 

C_DEPS += \
./ECU.d \
./ICU.d \
./TIMERS.d \
./TWI.d \
./UART.d \
./external_eeprom.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


