################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01.c \
C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_tsensor.c 

OBJS += \
./Drivers/BSP/stm32l4s5i_iot01.o \
./Drivers/BSP/stm32l4s5i_iot01_tsensor.o 

C_DEPS += \
./Drivers/BSP/stm32l4s5i_iot01.d \
./Drivers/BSP/stm32l4s5i_iot01_tsensor.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/stm32l4s5i_iot01.o: C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/stm32l4s5i_iot01.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/stm32l4s5i_iot01_tsensor.o: C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Drivers/BSP/B-L4S5I-IOT01/stm32l4s5i_iot01_tsensor.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/stm32l4s5i_iot01_tsensor.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

