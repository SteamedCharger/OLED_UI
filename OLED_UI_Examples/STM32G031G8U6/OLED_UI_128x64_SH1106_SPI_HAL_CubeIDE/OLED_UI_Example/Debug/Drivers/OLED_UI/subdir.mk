################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/OLED_UI/OLED.c \
../Drivers/OLED_UI/OLED_Fonts.c \
../Drivers/OLED_UI/OLED_UI.c \
../Drivers/OLED_UI/OLED_UI_Driver.c \
../Drivers/OLED_UI/OLED_UI_MenuData.c \
../Drivers/OLED_UI/OLED_driver.c 

OBJS += \
./Drivers/OLED_UI/OLED.o \
./Drivers/OLED_UI/OLED_Fonts.o \
./Drivers/OLED_UI/OLED_UI.o \
./Drivers/OLED_UI/OLED_UI_Driver.o \
./Drivers/OLED_UI/OLED_UI_MenuData.o \
./Drivers/OLED_UI/OLED_driver.o 

C_DEPS += \
./Drivers/OLED_UI/OLED.d \
./Drivers/OLED_UI/OLED_Fonts.d \
./Drivers/OLED_UI/OLED_UI.d \
./Drivers/OLED_UI/OLED_UI_Driver.d \
./Drivers/OLED_UI/OLED_UI_MenuData.d \
./Drivers/OLED_UI/OLED_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/OLED_UI/%.o Drivers/OLED_UI/%.su Drivers/OLED_UI/%.cyclo: ../Drivers/OLED_UI/%.c Drivers/OLED_UI/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -DUSE_FULL_LL_DRIVER -c -I../User -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Drivers/OLED_UI -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-OLED_UI

clean-Drivers-2f-OLED_UI:
	-$(RM) ./Drivers/OLED_UI/OLED.cyclo ./Drivers/OLED_UI/OLED.d ./Drivers/OLED_UI/OLED.o ./Drivers/OLED_UI/OLED.su ./Drivers/OLED_UI/OLED_Fonts.cyclo ./Drivers/OLED_UI/OLED_Fonts.d ./Drivers/OLED_UI/OLED_Fonts.o ./Drivers/OLED_UI/OLED_Fonts.su ./Drivers/OLED_UI/OLED_UI.cyclo ./Drivers/OLED_UI/OLED_UI.d ./Drivers/OLED_UI/OLED_UI.o ./Drivers/OLED_UI/OLED_UI.su ./Drivers/OLED_UI/OLED_UI_Driver.cyclo ./Drivers/OLED_UI/OLED_UI_Driver.d ./Drivers/OLED_UI/OLED_UI_Driver.o ./Drivers/OLED_UI/OLED_UI_Driver.su ./Drivers/OLED_UI/OLED_UI_MenuData.cyclo ./Drivers/OLED_UI/OLED_UI_MenuData.d ./Drivers/OLED_UI/OLED_UI_MenuData.o ./Drivers/OLED_UI/OLED_UI_MenuData.su ./Drivers/OLED_UI/OLED_driver.cyclo ./Drivers/OLED_UI/OLED_driver.d ./Drivers/OLED_UI/OLED_driver.o ./Drivers/OLED_UI/OLED_driver.su

.PHONY: clean-Drivers-2f-OLED_UI

