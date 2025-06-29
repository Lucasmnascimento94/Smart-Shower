################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lib/DMA/dma.c 

OBJS += \
./Core/Src/lib/DMA/dma.o 

C_DEPS += \
./Core/Src/lib/DMA/dma.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lib/DMA/%.o Core/Src/lib/DMA/%.su Core/Src/lib/DMA/%.cyclo: ../Core/Src/lib/DMA/%.c Core/Src/lib/DMA/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Smart-Shower/smartValve/Core/Src/lib" -I"C:/Smart-Shower/smartValve/Core/Src/lib/definitions" -I"C:/Smart-Shower/smartValve/Core/Src/lib/ds18b20_lib" -I"C:/Smart-Shower/smartValve/Core/Src/lib/equipments" -I"C:/Smart-Shower/smartValve/Core/Src/lib/ESP" -I"C:/Smart-Shower/smartValve/Core/Src/lib/screen" -I"C:/Smart-Shower/smartValve/Core/Src/lib/serial_printing" -I"C:/Smart-Shower/smartValve/Core/Src/lib/timer_us" -I"C:/Smart-Shower/smartValve/Core/Src/lib/screen/USER" -I"C:/Smart-Shower/smartValve/Core/Src/lib/screen/LCD" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Smart-Shower/smartValve/Core/Src/lib/UART_FUNC" -I"C:/Smart-Shower/smartValve/Core/Src/lib/DMA" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lib-2f-DMA

clean-Core-2f-Src-2f-lib-2f-DMA:
	-$(RM) ./Core/Src/lib/DMA/dma.cyclo ./Core/Src/lib/DMA/dma.d ./Core/Src/lib/DMA/dma.o ./Core/Src/lib/DMA/dma.su

.PHONY: clean-Core-2f-Src-2f-lib-2f-DMA

