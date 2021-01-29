################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/WiFi_HTTP_Server/Src/flash_l4.c \
C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/WiFi_HTTP_Server/Src/main.c \
C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/WiFi_HTTP_Server/Src/stm32l4xx_it.c \
../Application/User/syscalls.c \
../Application/User/sysmem.c \
C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/WiFi_HTTP_Server/Src/system_stm32l4xx.c 

OBJS += \
./Application/User/flash_l4.o \
./Application/User/main.o \
./Application/User/stm32l4xx_it.o \
./Application/User/syscalls.o \
./Application/User/sysmem.o \
./Application/User/system_stm32l4xx.o 

C_DEPS += \
./Application/User/flash_l4.d \
./Application/User/main.d \
./Application/User/stm32l4xx_it.d \
./Application/User/syscalls.d \
./Application/User/sysmem.d \
./Application/User/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/flash_l4.o: C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/WiFi_HTTP_Server/Src/flash_l4.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/flash_l4.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/main.o: C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/WiFi_HTTP_Server/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32l4xx_it.o: C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/WiFi_HTTP_Server/Src/stm32l4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/stm32l4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/syscalls.o: ../Application/User/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/sysmem.o: ../Application/User/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/system_stm32l4xx.o: C:/Users/albin/Downloads/en.stm32cubel4_v1-16-0/STM32Cube_FW_L4_V1.16.0/Projects/B-L4S5I-IOT01A/Applications/WiFi/WiFi_HTTP_Server/Src/system_stm32l4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Application/User/system_stm32l4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

