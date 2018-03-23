################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/rtos/mqx/mqx/source/nio/fs/fs_supp.c 

OBJS += \
./SDK/rtos/mqx/mqx/source/nio/fs/fs_supp.o 

C_DEPS += \
./SDK/rtos/mqx/mqx/source/nio/fs/fs_supp.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/mqx/mqx/source/nio/fs/%.o: ../SDK/rtos/mqx/mqx/source/nio/fs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK64FN1M0VLL12" -D"FSL_RTOS_MQX" -D"PEX_MQX_KSDK" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/hal/inc" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/hal/src/sim/MK64F12" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/system/src/clock/MK64F12" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/system/inc" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/osa/inc" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/CMSIS/Include" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/devices" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/devices/MK64F12/include" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/utilities/src" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/utilities/inc" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/devices/MK64F12/startup" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/Generated_Code/SDK/platform/devices/MK64F12/startup" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/Sources" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/Generated_Code" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/drivers/inc" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/psp/cortex_m" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/psp/cortex_m/cpu" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/config/common" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/include" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/bsp" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/psp/cortex_m/compiler/gcc_arm" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio/src" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio/fs" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio/drivers/nio_dummy" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio/drivers/nio_serial" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio/drivers/nio_tty" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx_stdlib/source/include" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx_stdlib/source/stdio" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/hal/src/uart" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/SDK/platform/drivers/src/uart" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/Generated_Code/SDK/rtos/mqx/config/board" -I"//samba4.engr.uvic.ca/asinha/Downloads/ceng455Proj3/DDS/Generated_Code/SDK/rtos/mqx/config/mcu" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


