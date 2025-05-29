################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lib/screen/LCD/FONT.c \
../Core/Src/lib/screen/LCD/lcd.c 

OBJS += \
./Core/Src/lib/screen/LCD/FONT.o \
./Core/Src/lib/screen/LCD/lcd.o 

C_DEPS += \
./Core/Src/lib/screen/LCD/FONT.d \
./Core/Src/lib/screen/LCD/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lib/screen/LCD/%.o Core/Src/lib/screen/LCD/%.su Core/Src/lib/screen/LCD/%.cyclo: ../Core/Src/lib/screen/LCD/%.c Core/Src/lib/screen/LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Include -I"C:/StartTech/Slave/Core/Src/lib/ds18b20_lib" -I"C:/StartTech/Slave/Core/Src/lib/serial_printing" -I"C:/StartTech/Slave/Core/Src/lib/timer_us" -I"C:/StartTech/Slave/Core/Src/lib/equipments" -I"C:/StartTech/Slave/Core/Src/lib/definitions" -I"C:/StartTech/Slave/Core/Src/lib/screen" -I"C:/StartTech/Slave/Core/Src/lib/screen/USER" -I"C:/StartTech/Slave/Core/Src/lib/screen/LED" -I"C:/StartTech/Slave/Core/Src/lib/screen/LCD" -I"C:/StartTech/Slave/Core/Src/lib/ESP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lib-2f-screen-2f-LCD

clean-Core-2f-Src-2f-lib-2f-screen-2f-LCD:
	-$(RM) ./Core/Src/lib/screen/LCD/FONT.cyclo ./Core/Src/lib/screen/LCD/FONT.d ./Core/Src/lib/screen/LCD/FONT.o ./Core/Src/lib/screen/LCD/FONT.su ./Core/Src/lib/screen/LCD/lcd.cyclo ./Core/Src/lib/screen/LCD/lcd.d ./Core/Src/lib/screen/LCD/lcd.o ./Core/Src/lib/screen/LCD/lcd.su

.PHONY: clean-Core-2f-Src-2f-lib-2f-screen-2f-LCD

