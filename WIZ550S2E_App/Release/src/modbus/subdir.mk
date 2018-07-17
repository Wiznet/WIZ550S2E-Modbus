################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/modbus/mb.c \
../src/modbus/mbascii.c \
../src/modbus/mbcrc.c \
../src/modbus/mbrtu.c \
../src/modbus/mbtcp.c \
../src/modbus/mbtimer.c 

OBJS += \
./src/modbus/mb.o \
./src/modbus/mbascii.o \
./src/modbus/mbcrc.o \
./src/modbus/mbrtu.o \
./src/modbus/mbtcp.o \
./src/modbus/mbtimer.o 

C_DEPS += \
./src/modbus/mb.d \
./src/modbus/mbascii.d \
./src/modbus/mbcrc.d \
./src/modbus/mbrtu.d \
./src/modbus/mbtcp.d \
./src/modbus/mbtimer.d 


# Each subdirectory must supply rules for building sources it contributes
src/modbus/%.o: ../src/modbus/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M0 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC11EXX__ -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\netutil" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\callback" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\ATcmd" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\Configuration" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\Ethernet" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\Internet" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\Internet\MQTT\MQTTPacket\src" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\PlatformHandler" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\S2E" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\wiznet_s2e_wiz550s2e_board\inc" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\lpc_chip_11exx\inc" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\modbus" -O3 -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


