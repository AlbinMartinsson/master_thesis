################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/test/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/xilinx/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/ti/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/st/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/pic32/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/nxp/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/mynewt/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/intel/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/devcrypto/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/cypress/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/cavium/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/caam/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/atmel/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/arm/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/af_alg/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/Renesas/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/Espressif/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/port/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/src/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/benchmark/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ctaocrypt/src/subdir.mk
-include Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/IDE/STM32Cube/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/subdir.mk
-include Drivers/BSP/Components/HTS221/subdir.mk
-include Drivers/BSP/B-L475E-IOT01/subdir.mk
-include Drivers/BSP/subdir.mk
-include Application/WiFi/subdir.mk
-include Application/User/subdir.mk
-include Application/Startup/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

BUILD_ARTIFACT_NAME := WIFI_WOLFSSL
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX := 
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME).$(BUILD_ARTIFACT_EXTENSION)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
WIFI_WOLFSSL.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
WIFI_WOLFSSL.list \

OBJCOPY_HEX += \
WIFI_WOLFSSL.hex \

OBJCOPY_BIN += \
WIFI_WOLFSSL.bin \


# All Target
all: main-build

# Main-build Target
main-build: WIFI_WOLFSSL.elf secondary-outputs

# Tool invocations
WIFI_WOLFSSL.elf: $(OBJS) $(USER_OBJS) C:\Users\albin\Downloads\en.stm32cubel4_v1-16-0(1)\STM32Cube_FW_L4_V1.16.0\Projects\B-L4S5I-IOT01A\Applications\WiFi\WiFi_HTTP_Server\WIFI_WOLFSSL\STM32L4S5VITX_FLASH.ld
	arm-none-eabi-gcc -o "WIFI_WOLFSSL.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\albin\Downloads\en.stm32cubel4_v1-16-0(1)\STM32Cube_FW_L4_V1.16.0\Projects\B-L4S5I-IOT01A\Applications\WiFi\WiFi_HTTP_Server\WIFI_WOLFSSL\STM32L4S5VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="WIFI_WOLFSSL.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

WIFI_WOLFSSL.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "WIFI_WOLFSSL.list"
	@echo 'Finished building: $@'
	@echo ' '

WIFI_WOLFSSL.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "WIFI_WOLFSSL.hex"
	@echo 'Finished building: $@'
	@echo ' '

WIFI_WOLFSSL.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "WIFI_WOLFSSL.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified
.SECONDARY:

-include ../makefile.targets
