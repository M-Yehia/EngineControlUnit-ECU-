################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Angle_PWM_Detection.c \
../ICU.c \
../INT.c \
../TIMERS.c \
../UART.c 

OBJS += \
./Angle_PWM_Detection.o \
./ICU.o \
./INT.o \
./TIMERS.o \
./UART.o 

C_DEPS += \
./Angle_PWM_Detection.d \
./ICU.d \
./INT.d \
./TIMERS.d \
./UART.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


