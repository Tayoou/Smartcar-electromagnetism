################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/iLLD/TC26B/Tricore/Port/Io/IfxPort_Io.c 

OBJS += \
./Libraries/iLLD/TC26B/Tricore/Port/Io/IfxPort_Io.o 

COMPILED_SRCS += \
./Libraries/iLLD/TC26B/Tricore/Port/Io/IfxPort_Io.src 

C_DEPS += \
./Libraries/iLLD/TC26B/Tricore/Port/Io/IfxPort_Io.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/iLLD/TC26B/Tricore/Port/Io/%.src: ../Libraries/iLLD/TC26B/Tricore/Port/Io/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gpt12" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/src/AppSw/Tricore/Driver" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/src/AppSw/Tricore/Main" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/src/AppSw/Tricore/User" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/src/AppSw/Tricore/APP" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/src/AppSw" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Infra/Platform/Tricore/Compilers" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Multican/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Infra/Platform" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Cif/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Hssl/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Cpu/Trap" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric/If/Ccu6If" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Dsadc/Dsadc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Port" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Stm/Timer" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Dts/Dts" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Eth" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Flash" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Vadc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Msc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Qspi/SpiMaster" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Scu/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric/SysSe/Comm" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric/SysSe/Math" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Infra/Platform/Tricore" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Trig" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Tim" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Ccu6/TimerWithTrigger" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Emem" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Mtu" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Infra" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Fft" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/I2c/I2c" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Asclin/Asc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric/SysSe" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Flash/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric/If" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Psi5" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Cpu" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Fce/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Stm/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Msc/Msc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Vadc/Adc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Asclin" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/Pwm" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Atom" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Port/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Psi5/Psi5" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Eray" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Qspi/SpiSlave" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Ccu6/Icu" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Cpu/CStart" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Hssl" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Cif" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Eth/Phy_Pef7071" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Hssl/Hssl" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Iom/Driver" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Multican/Can" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Psi5s/Psi5s" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Fft/Fft" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Ccu6/PwmHl" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Iom/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/_Lib" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Sent" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Eray/Eray" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gpt12/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Dma" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Fce/Crc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Qspi" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Infra/Sfr" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Infra/Sfr/TC26B" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric/SysSe/Bsp" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric/SysSe/General" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Cpu/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Dts" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Src" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Dma/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Cif/Cam" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Src/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Asclin/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/I2c/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Configurations" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/_Lib/DataHandling" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Sent/Sent" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Ccu6/Timer" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Psi5/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Psi5s" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Emem/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Ccu6/PwmBc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Iom" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Ccu6/TPwm" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Multican" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Mtu/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Infra/Sfr/TC26B/_Reg" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/PwmHl" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Dma/Dma" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/Timer" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Ccu6/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric/SysSe/Time" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Dsadc/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Cpu/Irq" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Ccu6" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gpt12/IncrEnc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Psi5s/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Scu" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/_Lib/InternalMux" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Stm" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Dsadc/Rdc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Vadc/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Dts/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Eth/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Smu" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/_PinMap" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Asclin/Lin" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric/StdIf" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Dsadc" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Fce" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/PwmHl" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Qspi/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Tom" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Tim/In" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Msc/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Fft/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/Pwm" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/Service/CpuGeneric/_Utilities" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Gtm/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Smu/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/I2c" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Asclin/Spi" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Eray/Std" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Port/Io" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/_Impl" -I"D:/LQ_TC26xB_LIB/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/My_test_official/Libraries/iLLD/TC26B/Tricore/Sent/Std" --iso=99 --c++14 --language=+volatile --anachronisms --fp-model=3 --fp-model=c --fp-model=f --fp-model=l --fp-model=n --fp-model=r --fp-model=z -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file=$(@:.src=.d) --misrac-version=2012 -N0 -Z0 -Y0 2>&1; sed -i -e '/ctc\\include/d' -e '/Libraries\\iLLD/d' -e '/Libraries\\Infra/d' -e 's/\(.*\)".*\\My_test_official\(\\.*\)"/\1\.\.\2/g' -e 's/\\/\//g' $(@:.src=.d) && \
	echo $(@:.src=.d) generated
	@echo 'Finished building: $<'
	@echo ' '

Libraries/iLLD/TC26B/Tricore/Port/Io/%.o: ./Libraries/iLLD/TC26B/Tricore/Port/Io/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


