################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../butt.s \
../buzz.s \
../colo.s \
../dast.s \
../eepr.s \
../lcdc.s \
../ledc.s \
../lise.s \
../main.s \
../menu.s \
../mstmIface.s \
../pomm.s \
../sche.s \
../seri.s \
../spii.s \
../stmt.s 

C_SRCS += \
../butt.c \
../buzz.c \
../colo.c \
../dast.c \
../eepr.c \
../lcdc.c \
../ledc.c \
../lica.c \
../lise.c \
../main.c \
../menu.c \
../mstmIface.c \
../pomm.c \
../relc.c \
../sche.c \
../seri.c \
../sett.c \
../spii.c \
../stat.c \
../stmt.c \
../such.c \
../util.c 

OBJS += \
./butt.o \
./buzz.o \
./colo.o \
./dast.o \
./eepr.o \
./lcdc.o \
./ledc.o \
./lica.o \
./lise.o \
./main.o \
./menu.o \
./mstmIface.o \
./pomm.o \
./relc.o \
./sche.o \
./seri.o \
./sett.o \
./spii.o \
./stat.o \
./stmt.o \
./such.o \
./util.o 

S_DEPS += \
./butt.d \
./buzz.d \
./colo.d \
./dast.d \
./eepr.d \
./lcdc.d \
./ledc.d \
./lise.d \
./main.d \
./menu.d \
./mstmIface.d \
./pomm.d \
./sche.d \
./seri.d \
./spii.d \
./stmt.d 

C_DEPS += \
./butt.d \
./buzz.d \
./colo.d \
./dast.d \
./eepr.d \
./lcdc.d \
./ledc.d \
./lica.d \
./lise.d \
./main.d \
./menu.d \
./mstmIface.d \
./pomm.d \
./relc.d \
./sche.d \
./seri.d \
./sett.d \
./spii.d \
./stat.d \
./stmt.d \
./such.d \
./util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.s
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -g2 -gstabs -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


