################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32L4xx_HAL_Driver/subdir.mk
-include Drivers/BSP/Components/HTS221/subdir.mk
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

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
WiFi_HTTP_Server.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
WiFi_HTTP_Server.list \

OBJCOPY_HEX += \
WiFi_HTTP_Server.hex \

OBJCOPY_BIN += \
WiFi_HTTP_Server.bin \


# All Target
all: WiFi_HTTP_Server.elf secondary-outputs

# Tool invocations
WiFi_HTTP_Server.elf: $(OBJS) $(USER_OBJS) C:\Users\albin\Downloads\en.stm32cubel4_v1-16-0\STM32Cube_FW_L4_V1.16.0\Projects\B-L4S5I-IOT01A\Applications\WiFi\WiFi_HTTP_Server\STM32CubeIDE\STM32L4S5VITX_FLASH.ld
	arm-none-eabi-gcc -o "WiFi_HTTP_Server.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\albin\Downloads\en.stm32cubel4_v1-16-0\STM32Cube_FW_L4_V1.16.0\Projects\B-L4S5I-IOT01A\Applications\WiFi\WiFi_HTTP_Server\STM32CubeIDE\STM32L4S5VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="WiFi_HTTP_Server.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

WiFi_HTTP_Server.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "WiFi_HTTP_Server.list"
	@echo 'Finished building: $@'
	@echo ' '

WiFi_HTTP_Server.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "WiFi_HTTP_Server.hex"
	@echo 'Finished building: $@'
	@echo ' '

WiFi_HTTP_Server.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "WiFi_HTTP_Server.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
