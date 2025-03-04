################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Flash/flash.c 

OBJS += \
./Drivers/Flash/flash.o 

C_DEPS += \
./Drivers/Flash/flash.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Flash/%.o Drivers/Flash/%.su Drivers/Flash/%.cyclo: ../Drivers/Flash/%.c Drivers/Flash/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -DUSE_FULL_LL_DRIVER -c -I../User -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Drivers/ACC -I../Drivers/ADS1231 -I../Drivers/SH1106 -I../Drivers/Timer -I../Drivers/Flash -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Flash

clean-Drivers-2f-Flash:
	-$(RM) ./Drivers/Flash/flash.cyclo ./Drivers/Flash/flash.d ./Drivers/Flash/flash.o ./Drivers/Flash/flash.su

.PHONY: clean-Drivers-2f-Flash

