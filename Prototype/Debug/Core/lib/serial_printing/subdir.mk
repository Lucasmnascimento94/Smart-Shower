################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/lib/serial_printing/serial_printing.c 

C_DEPS += \
./Core/lib/serial_printing/serial_printing.d 

OBJS += \
./Core/lib/serial_printing/serial_printing.o 


# Each subdirectory must supply rules for building sources it contributes
Core/lib/serial_printing/%.o Core/lib/serial_printing/%.su Core/lib/serial_printing/%.cyclo: ../Core/lib/serial_printing/%.c Core/lib/serial_printing/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F469xx -c -I../Core/Inc -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Include -I"C:/Users/lucas/OneDrive/Documentos/GitHub/Smart-Shower/Prototype/Core/lib/serial_printing" -I"C:/Users/lucas/OneDrive/Documentos/GitHub/Smart-Shower/Prototype/Core/lib/lcd" -I"C:/Users/lucas/OneDrive/Documentos/GitHub/Smart-Shower/Prototype/Core/Src" -I"C:/Users/lucas/OneDrive/Documentos/GitHub/Smart-Shower/Prototype/Core/lib/control" -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-lib-2f-serial_printing

clean-Core-2f-lib-2f-serial_printing:
	-$(RM) ./Core/lib/serial_printing/serial_printing.cyclo ./Core/lib/serial_printing/serial_printing.d ./Core/lib/serial_printing/serial_printing.o ./Core/lib/serial_printing/serial_printing.su

.PHONY: clean-Core-2f-lib-2f-serial_printing

