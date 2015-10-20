################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src-gen/subdir.mk
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
ExposureBox.lss \

FLASH_IMAGE += \
ExposureBox.hex \

EEPROM_IMAGE += \
ExposureBox.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: ExposureBox.elf secondary-outputs

# Tool invocations
ExposureBox.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,ExposureBox.map -mmcu=atmega32 -o "ExposureBox.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ExposureBox.lss: ExposureBox.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S ExposureBox.elf  >"ExposureBox.lss"
	@echo 'Finished building: $@'
	@echo ' '

ExposureBox.hex: ExposureBox.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex ExposureBox.elf  "ExposureBox.hex"
	@echo 'Finished building: $@'
	@echo ' '

ExposureBox.eep: ExposureBox.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex ExposureBox.elf  "ExposureBox.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ExposureBox.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 ExposureBox.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) ExposureBox.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
