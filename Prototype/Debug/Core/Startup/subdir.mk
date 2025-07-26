################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f469iitx.s 

S_DEPS += \
./Core/Startup/startup_stm32f469iitx.d 

OBJS += \
./Core/Startup/startup_stm32f469iitx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/lucas/OneDrive/Documentos/GitHub/Smart-Shower/Prototype/Core/lib/serial_printing" -I"C:/Users/lucas/OneDrive/Documentos/GitHub/Smart-Shower/Prototype/Core/lib/lcd" -I"C:/Users/lucas/OneDrive/Documentos/GitHub/Smart-Shower/Prototype/Core/Inc" -I"C:/Users/lucas/OneDrive/Documentos/GitHub/Smart-Shower/Prototype/Core/Src" -I"C:/Users/lucas/OneDrive/Documentos/GitHub/Smart-Shower/Prototype/Core/lib/control" -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f469iitx.d ./Core/Startup/startup_stm32f469iitx.o

.PHONY: clean-Core-2f-Startup

