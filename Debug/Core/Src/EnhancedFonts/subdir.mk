################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/EnhancedFonts/arialBlack_20ptFontInfo.c 

OBJS += \
./Core/Src/EnhancedFonts/arialBlack_20ptFontInfo.o 

C_DEPS += \
./Core/Src/EnhancedFonts/arialBlack_20ptFontInfo.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/EnhancedFonts/%.o Core/Src/EnhancedFonts/%.su: ../Core/Src/EnhancedFonts/%.c Core/Src/EnhancedFonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../LIBJPEG/App -I../LIBJPEG/Target -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/Third_Party/LibJPEG/include -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-EnhancedFonts

clean-Core-2f-Src-2f-EnhancedFonts:
	-$(RM) ./Core/Src/EnhancedFonts/arialBlack_20ptFontInfo.d ./Core/Src/EnhancedFonts/arialBlack_20ptFontInfo.o ./Core/Src/EnhancedFonts/arialBlack_20ptFontInfo.su

.PHONY: clean-Core-2f-Src-2f-EnhancedFonts

