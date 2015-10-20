################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
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
./util.o 

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
./util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


