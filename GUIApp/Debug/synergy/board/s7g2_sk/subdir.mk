################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/board/s7g2_sk/bsp_init.c \
../synergy/board/s7g2_sk/bsp_leds.c \
../synergy/board/s7g2_sk/bsp_qspi.c 

C_DEPS += \
./synergy/board/s7g2_sk/bsp_init.d \
./synergy/board/s7g2_sk/bsp_leds.d \
./synergy/board/s7g2_sk/bsp_qspi.d 

OBJS += \
./synergy/board/s7g2_sk/bsp_init.o \
./synergy/board/s7g2_sk/bsp_leds.o \
./synergy/board/s7g2_sk/bsp_qspi.o 

SREC += \
GUIApp.srec 

MAP += \
GUIApp.map 


# Each subdirectory must supply rules for building sources it contributes
synergy/board/s7g2_sk/%.o: ../synergy/board/s7g2_sk/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g3 -DSF_MESSAGE_CLASS -I../GUIApp/synergy_cfg/ssp_cfg/bsp/ -I../GUIApp/synergy_cfg/ssp_cfg/driver/ -I../GUIApp/synergy/ssp/inc/ -I../GUIApp/synergy/ssp/inc/bsp/ -I../GUIApp/synergy/ssp/inc/bsp/cmsis/Include/ -I../GUIApp/synergy/ssp/inc/driver/api/ -I../GUIApp/synergy/ssp/inc/driver/instances/ -I../GUIApp/synergy_cfg/ssp_cfg/framework/el/ -I../GUIApp/synergy/ssp/inc/framework/el/ -I../GUIApp/synergy/ssp/src/framework/el/tx/ -I../GUIApp/synergy_cfg/ssp_cfg/framework/ -I../GUIApp/synergy/ssp/inc/framework/api/ -I../GUIApp/synergy/ssp/inc/framework/instances/ -I../GUIApp/synergy_cfg/ssp_cfg/framework/tes/ -I../GUIApp/synergy/ssp/inc/framework/tes/ -I../GUIApp/synergy_cfg/framework/ -I../GUIApp/synergy_cfg/ssp_cfg/framework -I../GUIApp/synergy/ssp/inc/framework/api -I../GUIApp/synergy/ssp/inc/framework/instances -I../GUIApp/synergy_cfg/ssp_cfg/framework/tes -I../GUIApp/synergy/ssp/inc/framework/tes -I../GUIApp/synergy_cfg/ssp_cfg/framework/el -I../GUIApp/synergy/ssp/inc/framework/el -I../GUIApp/synergy/ssp/src/framework/el/tx -I../GUIApp/synergy_cfg/ssp_cfg/bsp -I../GUIApp/synergy_cfg/ssp_cfg/driver -I../GUIApp/synergy/ssp/inc -I../GUIApp/synergy/ssp/inc/bsp -I../GUIApp/synergy/ssp/inc/bsp/cmsis/Include -I../GUIApp/synergy/ssp/inc/driver/api -I../GUIApp/synergy/ssp/inc/driver/instances -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy_cfg/ssp_cfg/framework" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc/framework/api" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc/framework/instances" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc/framework/tes" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy_cfg/ssp_cfg/framework/el" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc/framework/el" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/src/framework/el/tx" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy_cfg/ssp_cfg/bsp" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy_cfg/ssp_cfg/driver" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc/bsp" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc/bsp/cmsis/Include" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc/driver/api" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc/driver/instances" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/src" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/src/synergy_gen" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp_supplemental/inc/framework/instances" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/src/framework/el/ux" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/src/framework/el/tx/tx_src" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/src/framework/sf_el_nx/phy" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc/framework/el/nxd" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/src/framework/el/nxd" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/inc/framework/el/nxd_application_layer" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/src/framework/el/nxd/nxd_src" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/src/framework/el/nx_md5" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/src/framework/sf_crypto" -I"C:/Users/MSI-VAMPIRIC/Desktop/s7g2/GUIApp/synergy/ssp/src/driver/r_sce" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

