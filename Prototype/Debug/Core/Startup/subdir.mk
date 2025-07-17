################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f411rctx.s 

S_DEPS += \
./Core/Startup/startup_stm32f411rctx.d 

OBJS += \
./Core/Startup/startup_stm32f411rctx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Smart-Shower/smartValve/Core/Src/lib" -I"C:/Smart-Shower/smartValve/Core/Src/lib/definitions" -I"C:/Smart-Shower/smartValve/Core/Src/lib/ds18b20_lib" -I"C:/Smart-Shower/smartValve/Core/Src/lib/equipments" -I"C:/Smart-Shower/smartValve/Core/Src/lib/ESP" -I"C:/Smart-Shower/smartValve/Core/Src/lib/screen" -I"C:/Smart-Shower/smartValve/Core/Src/lib/serial_printing" -I"C:/Smart-Shower/smartValve/Core/Src/lib/timer_us" -I"C:/Smart-Shower/smartValve/Core/Src/lib/screen/USER" -I"C:/Smart-Shower/smartValve/Core/Src/lib/screen/LCD" -I"C:/Smart-Shower/smartValve/Core/Src/lib/UART_FUNC" -I"C:/Smart-Shower/smartValve/Core/Src/lib/DMA" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f411rctx.d ./Core/Startup/startup_stm32f411rctx.o

.PHONY: clean-Core-2f-Startup

