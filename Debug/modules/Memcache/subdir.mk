################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../modules/Memcache/memcache.cpp 

OBJS += \
./modules/Memcache/memcache.o 

CPP_DEPS += \
./modules/Memcache/memcache.d 


# Each subdirectory must supply rules for building sources it contributes
modules/Memcache/%.o: ../modules/Memcache/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


