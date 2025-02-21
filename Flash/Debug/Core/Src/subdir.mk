################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/am1002.c \
../Core/Src/flash.c \
../Core/Src/function.c \
../Core/Src/main.c \
../Core/Src/modbus.c \
../Core/Src/parser.c \
../Core/Src/parser_handle_modbus.c \
../Core/Src/sensor.c \
../Core/Src/stm32f0xx_hal_msp.c \
../Core/Src/stm32f0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f0xx.c \
../Core/Src/timer.c \
../Core/Src/uart_handle.c 

OBJS += \
./Core/Src/am1002.o \
./Core/Src/flash.o \
./Core/Src/function.o \
./Core/Src/main.o \
./Core/Src/modbus.o \
./Core/Src/parser.o \
./Core/Src/parser_handle_modbus.o \
./Core/Src/sensor.o \
./Core/Src/stm32f0xx_hal_msp.o \
./Core/Src/stm32f0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f0xx.o \
./Core/Src/timer.o \
./Core/Src/uart_handle.o 

C_DEPS += \
./Core/Src/am1002.d \
./Core/Src/flash.d \
./Core/Src/function.d \
./Core/Src/main.d \
./Core/Src/modbus.d \
./Core/Src/parser.d \
./Core/Src/parser_handle_modbus.d \
./Core/Src/sensor.d \
./Core/Src/stm32f0xx_hal_msp.d \
./Core/Src/stm32f0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f0xx.d \
./Core/Src/timer.d \
./Core/Src/uart_handle.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F071xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/am1002.cyclo ./Core/Src/am1002.d ./Core/Src/am1002.o ./Core/Src/am1002.su ./Core/Src/flash.cyclo ./Core/Src/flash.d ./Core/Src/flash.o ./Core/Src/flash.su ./Core/Src/function.cyclo ./Core/Src/function.d ./Core/Src/function.o ./Core/Src/function.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/modbus.cyclo ./Core/Src/modbus.d ./Core/Src/modbus.o ./Core/Src/modbus.su ./Core/Src/parser.cyclo ./Core/Src/parser.d ./Core/Src/parser.o ./Core/Src/parser.su ./Core/Src/parser_handle_modbus.cyclo ./Core/Src/parser_handle_modbus.d ./Core/Src/parser_handle_modbus.o ./Core/Src/parser_handle_modbus.su ./Core/Src/sensor.cyclo ./Core/Src/sensor.d ./Core/Src/sensor.o ./Core/Src/sensor.su ./Core/Src/stm32f0xx_hal_msp.cyclo ./Core/Src/stm32f0xx_hal_msp.d ./Core/Src/stm32f0xx_hal_msp.o ./Core/Src/stm32f0xx_hal_msp.su ./Core/Src/stm32f0xx_it.cyclo ./Core/Src/stm32f0xx_it.d ./Core/Src/stm32f0xx_it.o ./Core/Src/stm32f0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f0xx.cyclo ./Core/Src/system_stm32f0xx.d ./Core/Src/system_stm32f0xx.o ./Core/Src/system_stm32f0xx.su ./Core/Src/timer.cyclo ./Core/Src/timer.d ./Core/Src/timer.o ./Core/Src/timer.su ./Core/Src/uart_handle.cyclo ./Core/Src/uart_handle.d ./Core/Src/uart_handle.o ./Core/Src/uart_handle.su

.PHONY: clean-Core-2f-Src

