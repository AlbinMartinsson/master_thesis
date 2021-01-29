################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/Common/Src/es_wifi.c \
C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/Common/Src/es_wifi_io.c \
C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/Common/Src/wifi.c 

OBJS += \
./Application/WiFi/es_wifi.o \
./Application/WiFi/es_wifi_io.o \
./Application/WiFi/wifi.o 

C_DEPS += \
./Application/WiFi/es_wifi.d \
./Application/WiFi/es_wifi_io.d \
./Application/WiFi/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
Application/WiFi/es_wifi.o: C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/Common/Src/es_wifi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/WiFi/es_wifi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/WiFi/es_wifi_io.o: C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/Common/Src/es_wifi_io.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/WiFi/es_wifi_io.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/WiFi/wifi.o: C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/Common/Src/wifi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/WiFi/wifi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

