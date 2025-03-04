################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Timer/soft_timer.c 

OBJS += \
./Drivers/Timer/soft_timer.o 

C_DEPS += \
./Drivers/Timer/soft_timer.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Timer/%.o Drivers/Timer/%.su Drivers/Timer/%.cyclo: ../Drivers/Timer/%.c Drivers/Timer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -DUSE_FULL_LL_DRIVER -c -I../User -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Drivers/ACC -I../Drivers/ADS1231 -I../Drivers/SH1106 -I../Drivers/Timer -I../Drivers/Flash -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Timer

clean-Drivers-2f-Timer:
	-$(RM) ./Drivers/Timer/soft_timer.cyclo ./Drivers/Timer/soft_timer.d ./Drivers/Timer/soft_timer.o ./Drivers/Timer/soft_timer.su

.PHONY: clean-Drivers-2f-Timer

