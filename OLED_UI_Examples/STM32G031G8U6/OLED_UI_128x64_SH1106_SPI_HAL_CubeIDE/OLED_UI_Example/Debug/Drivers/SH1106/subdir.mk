################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SH1106/oled.c 

OBJS += \
./Drivers/SH1106/oled.o 

C_DEPS += \
./Drivers/SH1106/oled.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SH1106/%.o Drivers/SH1106/%.su Drivers/SH1106/%.cyclo: ../Drivers/SH1106/%.c Drivers/SH1106/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -DUSE_FULL_LL_DRIVER -c -I../User -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Drivers/ACC -I../Drivers/ADS1231 -I../Drivers/SH1106 -I../Drivers/Timer -I../Drivers/Flash -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-SH1106

clean-Drivers-2f-SH1106:
	-$(RM) ./Drivers/SH1106/oled.cyclo ./Drivers/SH1106/oled.d ./Drivers/SH1106/oled.o ./Drivers/SH1106/oled.su

.PHONY: clean-Drivers-2f-SH1106

