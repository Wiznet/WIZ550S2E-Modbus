################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/PlatformHandler/eepromHandler.c \
../src/PlatformHandler/sspHandler.c \
../src/PlatformHandler/timerHandler.c \
../src/PlatformHandler/uartHandler.c 

OBJS += \
./src/PlatformHandler/eepromHandler.o \
./src/PlatformHandler/sspHandler.o \
./src/PlatformHandler/timerHandler.o \
./src/PlatformHandler/uartHandler.o 

C_DEPS += \
./src/PlatformHandler/eepromHandler.d \
./src/PlatformHandler/sspHandler.d \
./src/PlatformHandler/timerHandler.d \
./src/PlatformHandler/uartHandler.d 


# Each subdirectory must supply rules for building sources it contributes
src/PlatformHandler/%.o: ../src/PlatformHandler/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M0 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC11EXX__ -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\netutil" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\callback" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\ATcmd" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\Configuration" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\Ethernet" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\Internet" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\Internet\MQTT\MQTTPacket\src" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\PlatformHandler" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\S2E" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\wiznet_s2e_wiz550s2e_board\inc" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\lpc_chip_11exx\inc" -I"C:\Users\boomw\Documents\lpcxpresso-workspace-wiz550s2e\WIZ550S2E_App\src\modbus" -O3 -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


