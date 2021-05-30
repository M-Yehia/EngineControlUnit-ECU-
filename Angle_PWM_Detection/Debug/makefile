################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Angle_PWM_Detection.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: Angle_PWM_Detection.elf secondary-outputs

# Tool invocations
Angle_PWM_Detection.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Angle_PWM_Detection.map -mmcu=atmega128 -o "Angle_PWM_Detection.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Angle_PWM_Detection.lss: Angle_PWM_Detection.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Angle_PWM_Detection.elf  >"Angle_PWM_Detection.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Angle_PWM_Detection.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128 Angle_PWM_Detection.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Angle_PWM_Detection.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
