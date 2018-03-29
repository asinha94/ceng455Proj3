################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/rtos/mqx/mqx/source/string/str_utos.c \
../SDK/rtos/mqx/mqx/source/string/strnlen.c 

OBJS += \
./SDK/rtos/mqx/mqx/source/string/str_utos.o \
./SDK/rtos/mqx/mqx/source/string/strnlen.o 

C_DEPS += \
./SDK/rtos/mqx/mqx/source/string/str_utos.d \
./SDK/rtos/mqx/mqx/source/string/strnlen.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/mqx/mqx/source/string/%.o: ../SDK/rtos/mqx/mqx/source/string/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK64FN1M0VLL12" -D"FSL_RTOS_MQX" -D"PEX_MQX_KSDK" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/hal/inc" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/hal/src/sim/MK64F12" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/system/src/clock/MK64F12" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/system/inc" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/osa/inc" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/CMSIS/Include" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/devices" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/devices/MK64F12/include" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/utilities/src" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/utilities/inc" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/devices/MK64F12/startup" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/Generated_Code/SDK/platform/devices/MK64F12/startup" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/Sources" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/Generated_Code" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/drivers/inc" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/psp/cortex_m" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/psp/cortex_m/cpu" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/config/common" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/include" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/bsp" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/psp/cortex_m/compiler/gcc_arm" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/nio" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/nio/src" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/nio/fs" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/nio/drivers/nio_dummy" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/nio/drivers/nio_serial" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx/source/nio/drivers/nio_tty" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx_stdlib/source/include" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/rtos/mqx/mqx_stdlib/source/stdio" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/hal/src/uart" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/SDK/platform/drivers/src/uart" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/Generated_Code/SDK/rtos/mqx/config/board" -I"C:/Users/asinha/Downloads/ceng455proj3/DDS/Generated_Code/SDK/rtos/mqx/config/mcu" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


