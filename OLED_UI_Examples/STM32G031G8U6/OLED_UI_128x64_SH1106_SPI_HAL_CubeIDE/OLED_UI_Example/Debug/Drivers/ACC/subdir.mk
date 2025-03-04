################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ACC/LIS2DH12.c 

OBJS += \
./Drivers/ACC/LIS2DH12.o 

C_DEPS += \
./Drivers/ACC/LIS2DH12.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ACC/%.o Drivers/ACC/%.su Drivers/ACC/%.cyclo: ../Drivers/ACC/%.c Drivers/ACC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -DUSE_FULL_LL_DRIVER -c -I../User -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Drivers/ACC -I../Drivers/ADS1231 -I../Drivers/SH1106 -I../Drivers/Timer -I../Drivers/Flash -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-ACC

clean-Drivers-2f-ACC:
	-$(RM) ./Drivers/ACC/LIS2DH12.cyclo ./Drivers/ACC/LIS2DH12.d ./Drivers/ACC/LIS2DH12.o ./Drivers/ACC/LIS2DH12.su

.PHONY: clean-Drivers-2f-ACC

