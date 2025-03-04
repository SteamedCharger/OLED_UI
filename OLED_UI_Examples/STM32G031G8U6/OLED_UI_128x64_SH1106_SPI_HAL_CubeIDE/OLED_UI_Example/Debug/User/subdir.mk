################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/misc.c \
../User/user_it.c \
../User/usermain.c 

OBJS += \
./User/misc.o \
./User/user_it.o \
./User/usermain.o 

C_DEPS += \
./User/misc.d \
./User/user_it.d \
./User/usermain.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o User/%.su User/%.cyclo: ../User/%.c User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -DUSE_FULL_LL_DRIVER -c -I../User -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Drivers/OLED_UI -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User

clean-User:
	-$(RM) ./User/misc.cyclo ./User/misc.d ./User/misc.o ./User/misc.su ./User/user_it.cyclo ./User/user_it.d ./User/user_it.o ./User/user_it.su ./User/usermain.cyclo ./User/usermain.d ./User/usermain.o ./User/usermain.su

.PHONY: clean-User

