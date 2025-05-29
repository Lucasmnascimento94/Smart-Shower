################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Timer/timer_us.c 

OBJS += \
./Core/Src/Timer/timer_us.o 

C_DEPS += \
./Core/Src/Timer/timer_us.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Timer/%.o Core/Src/Timer/%.su Core/Src/Timer/%.cyclo: ../Core/Src/Timer/%.c Core/Src/Timer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F469xx -c -I../Core/Inc -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/lucas/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Include -I"C:/StartTech/Prototype/Core/Src/Timer" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Timer

clean-Core-2f-Src-2f-Timer:
	-$(RM) ./Core/Src/Timer/timer_us.cyclo ./Core/Src/Timer/timer_us.d ./Core/Src/Timer/timer_us.o ./Core/Src/Timer/timer_us.su

.PHONY: clean-Core-2f-Src-2f-Timer

