################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/lib/lcd/FONT.c \
../Core/lib/lcd/GUI.c \
../Core/lib/lcd/pages.c \
../Core/lib/lcd/touch.c 

OBJS += \
./Core/lib/lcd/FONT.o \
./Core/lib/lcd/GUI.o \
./Core/lib/lcd/pages.o \
./Core/lib/lcd/touch.o 

C_DEPS += \
./Core/lib/lcd/FONT.d \
./Core/lib/lcd/GUI.d \
./Core/lib/lcd/pages.d \
./Core/lib/lcd/touch.d 


# Each subdirectory must supply rules for building sources it contributes
Core/lib/lcd/%.o Core/lib/lcd/%.su Core/lib/lcd/%.cyclo: ../Core/lib/lcd/%.c Core/lib/lcd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F469xx -c -I../Core/Inc -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Include -I"C:/StartTech/Prototype/Core/lib/serial_printing" -I"C:/StartTech/Prototype/Core/lib/lcd" -I"C:/StartTech/Prototype/Core/Src" -I"C:/StartTech/Prototype/Core/lib/control" -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-lib-2f-lcd

clean-Core-2f-lib-2f-lcd:
	-$(RM) ./Core/lib/lcd/FONT.cyclo ./Core/lib/lcd/FONT.d ./Core/lib/lcd/FONT.o ./Core/lib/lcd/FONT.su ./Core/lib/lcd/GUI.cyclo ./Core/lib/lcd/GUI.d ./Core/lib/lcd/GUI.o ./Core/lib/lcd/GUI.su ./Core/lib/lcd/pages.cyclo ./Core/lib/lcd/pages.d ./Core/lib/lcd/pages.o ./Core/lib/lcd/pages.su ./Core/lib/lcd/touch.cyclo ./Core/lib/lcd/touch.d ./Core/lib/lcd/touch.o ./Core/lib/lcd/touch.su

.PHONY: clean-Core-2f-lib-2f-lcd

