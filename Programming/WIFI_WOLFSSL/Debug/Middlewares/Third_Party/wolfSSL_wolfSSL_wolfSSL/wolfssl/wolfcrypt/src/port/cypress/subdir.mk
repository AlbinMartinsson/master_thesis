################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/cypress/psoc6_crypto.c 

OBJS += \
./Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/cypress/psoc6_crypto.o 

C_DEPS += \
./Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/cypress/psoc6_crypto.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/cypress/psoc6_crypto.o: ../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/cypress/psoc6_crypto.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32L4S5I_IOT01 -DUSE_HAL_DRIVER -DSTM32L4S5xx -c -I../../../../../../../Drivers/BSP/B-L4S5I-IOT01 -I../../../Common/Inc -I../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Inc -I../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Drivers/CMSIS/Include -I"C:/Users/albin/Documents/GitHub/master_thesis/Programming/WolfSSLTest/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl" -I"C:/Users/albin/Documents/GitHub/master_thesis/Programming/WolfSSLTest/Core/Inc" -I"C:/Users/albin/Documents/GitHub/master_thesis/Programming/WolfSSLTest/wolfSSL" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/cypress/psoc6_crypto.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
