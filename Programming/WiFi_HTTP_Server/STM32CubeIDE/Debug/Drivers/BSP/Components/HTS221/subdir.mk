################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Drivers/BSP/Components/hts221/hts221.c 

OBJS += \
./Drivers/BSP/Components/HTS221/hts221.o 

C_DEPS += \
./Drivers/BSP/Components/HTS221/hts221.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/HTS221/hts221.o: C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Drivers/BSP/Components/hts221/hts221.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/HTS221/hts221.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

