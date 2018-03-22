################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/Cpu.c \
../Generated_Code/DD.c \
../Generated_Code/MainTask.c \
../Generated_Code/clockMan1.c \
../Generated_Code/fsl_hwtimer1.c \
../Generated_Code/fsl_mpu1.c \
../Generated_Code/hardware_init.c \
../Generated_Code/mqx_ksdk.c \
../Generated_Code/osa1.c \
../Generated_Code/pin_init.c \
../Generated_Code/uart1.c 

OBJS += \
./Generated_Code/Cpu.o \
./Generated_Code/DD.o \
./Generated_Code/MainTask.o \
./Generated_Code/clockMan1.o \
./Generated_Code/fsl_hwtimer1.o \
./Generated_Code/fsl_mpu1.o \
./Generated_Code/hardware_init.o \
./Generated_Code/mqx_ksdk.o \
./Generated_Code/osa1.o \
./Generated_Code/pin_init.o \
./Generated_Code/uart1.o 

C_DEPS += \
./Generated_Code/Cpu.d \
./Generated_Code/DD.d \
./Generated_Code/MainTask.d \
./Generated_Code/clockMan1.d \
./Generated_Code/fsl_hwtimer1.d \
./Generated_Code/fsl_mpu1.d \
./Generated_Code/hardware_init.d \
./Generated_Code/mqx_ksdk.d \
./Generated_Code/osa1.d \
./Generated_Code/pin_init.d \
./Generated_Code/uart1.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK64FN1M0VLL12" -D"FSL_RTOS_MQX" -D"PEX_MQX_KSDK" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/hal/inc" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/hal/src/sim/MK64F12" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/system/src/clock/MK64F12" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/system/inc" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/osa/inc" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/CMSIS/Include" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/devices" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/devices/MK64F12/include" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/utilities/src" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/utilities/inc" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/devices/MK64F12/startup" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/Generated_Code/SDK/platform/devices/MK64F12/startup" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/Sources" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/Generated_Code" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/drivers/inc" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/psp/cortex_m" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/psp/cortex_m/cpu" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/config/common" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/include" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/bsp" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/psp/cortex_m/compiler/gcc_arm" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio/src" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio/fs" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio/drivers/nio_dummy" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio/drivers/nio_serial" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx/source/nio/drivers/nio_tty" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx_stdlib/source/include" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/rtos/mqx/mqx_stdlib/source/stdio" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/hal/src/uart" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/SDK/platform/drivers/src/uart" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/Generated_Code/SDK/rtos/mqx/config/board" -I"C:/Users/asinha.UVIC.001/Downloads/ceng455Proj3/DDS/Generated_Code/SDK/rtos/mqx/config/mcu" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


