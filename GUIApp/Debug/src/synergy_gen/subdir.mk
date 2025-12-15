################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/synergy_gen/common_data.c \
../src/synergy_gen/hal_data.c \
../src/synergy_gen/main.c \
../src/synergy_gen/main_thread.c \
../src/synergy_gen/network_thread.c \
../src/synergy_gen/pin_data.c \
../src/synergy_gen/usb_thread.c 

C_DEPS += \
./src/synergy_gen/common_data.d \
./src/synergy_gen/hal_data.d \
./src/synergy_gen/main.d \
./src/synergy_gen/main_thread.d \
./src/synergy_gen/network_thread.d \
./src/synergy_gen/pin_data.d \
./src/synergy_gen/usb_thread.d 

OBJS += \
./src/synergy_gen/common_data.o \
./src/synergy_gen/hal_data.o \
./src/synergy_gen/main.o \
./src/synergy_gen/main_thread.o \
./src/synergy_gen/network_thread.o \
./src/synergy_gen/pin_data.o \
./src/synergy_gen/usb_thread.o 

SREC += \
GUIApp.srec 

MAP += \
GUIApp.map 


# Each subdirectory must supply rules for building sources it contributes
src/synergy_gen/%.o: ../src/synergy_gen/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g3 -DSF_MESSAGE_CLASS -I../GUIApp/synergy_cfg/ssp_cfg/bsp/ -I../GUIApp/synergy_cfg/ssp_cfg/driver/ -I../GUIApp/synergy/ssp/inc/ -I../GUIApp/synergy/ssp/inc/bsp/ -I../GUIApp/synergy/ssp/inc/bsp/cmsis/Include/ -I../GUIApp/synergy/ssp/inc/driver/api/ -I../GUIApp/synergy/ssp/inc/driver/instances/ -I../GUIApp/synergy_cfg/ssp_cfg/framework/el/ -I../GUIApp/synergy/ssp/inc/framework/el/ -I../GUIApp/synergy/ssp/src/framework/el/tx/ -I../GUIApp/synergy_cfg/ssp_cfg/framework/ -I../GUIApp/synergy/ssp/inc/framework/api/ -I../GUIApp/synergy/ssp/inc/framework/instances/ -I../GUIApp/synergy_cfg/ssp_cfg/framework/tes/ -I../GUIApp/synergy/ssp/inc/framework/tes/ -I../GUIApp/synergy_cfg/framework/ -I../GUIApp/synergy_cfg/ssp_cfg/framework -I../GUIApp/synergy/ssp/inc/framework/api -I../GUIApp/synergy/ssp/inc/framework/instances -I../GUIApp/synergy_cfg/ssp_cfg/framework/tes -I../GUIApp/synergy/ssp/inc/framework/tes -I../GUIApp/synergy_cfg/ssp_cfg/framework/el -I../GUIApp/synergy/ssp/inc/framework/el -I../GUIApp/synergy/ssp/src/framework/el/tx -I../GUIApp/synergy_cfg/ssp_cfg/bsp -I../GUIApp/synergy_cfg/ssp_cfg/driver -I../GUIApp/synergy/ssp/inc -I../GUIApp/synergy/ssp/inc/bsp -I../GUIApp/synergy/ssp/inc/bsp/cmsis/Include -I../GUIApp/synergy/ssp/inc/driver/api -I../GUIApp/synergy/ssp/inc/driver/instances -I"D:/Gitlab/s7g2/GUIApp/synergy_cfg/ssp_cfg/framework" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc/framework/api" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc/framework/instances" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc/framework/tes" -I"D:/Gitlab/s7g2/GUIApp/synergy_cfg/ssp_cfg/framework/el" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc/framework/el" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/src/framework/el/tx" -I"D:/Gitlab/s7g2/GUIApp/synergy_cfg/ssp_cfg/bsp" -I"D:/Gitlab/s7g2/GUIApp/synergy_cfg/ssp_cfg/driver" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc/bsp" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc/bsp/cmsis/Include" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc/driver/api" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc/driver/instances" -I"D:/Gitlab/s7g2/GUIApp/src" -I"D:/Gitlab/s7g2/GUIApp/src/synergy_gen" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp_supplemental/inc/framework/instances" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/src/framework/el/ux" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/src/framework/el/tx/tx_src" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/src/framework/sf_el_nx/phy" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc/framework/el/nxd" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/src/framework/el/nxd" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/inc/framework/el/nxd_application_layer" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/src/framework/el/nxd/nxd_src" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/src/framework/el/nx_md5" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/src/framework/sf_crypto" -I"D:/Gitlab/s7g2/GUIApp/synergy/ssp/src/driver/r_sce" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

